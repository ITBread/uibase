#include <unistd.h>  
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#include <sys/prctl.h>
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/mman.h>
#include <dirent.h>

#include "jpegcodec.h"

#include "drawjpeg.h"
#include "winbase.h"

#define MAXJPEGFILELEN		(128<<10)

typedef struct _Jpeg_base_codec_info 
{    
    int jpegcodechandle;
  
    int mmapdatalen;
    unsigned int out_virt_buffer;
    unsigned int jpeg_phybuffer_addr;

    jpeg_info_t *pjpeginfo; 
    unsigned char *pjpegfilebuf;

	pthread_mutex_t mutexhandle;
    
} Jpeg_base_codec_info ;

static unsigned char jpegdecoderflag = 0;
static Jpeg_base_codec_info jpegbaseinfo;


static int jpegcodec_open_base()
{
    unsigned char *pJpegBuffer;
    int i, rvalue, jpegcodechandle;

    unsigned char *pjpegfilebuf;
    Jpeg_base_codec_info *pbaseinfo = &jpegbaseinfo;
   
    char devname[16];
    memset(devname, 0, 16);
    strcpy(devname, "/dev/video0");

    if(jpegdecoderflag)
    {
        printf("%s %d  Error jpegdecoder_open\n",  __FUNCTION__,__LINE__);
        return 1;
    }

    DIR *dir = opendir("/sys/class/video4linux/video1/");
	if(dir)
	{
		closedir(dir);		
		strcpy(devname, "/dev/video1");
	}	
    
	printf("jpegcodec is %s\n", devname);	

	for(i=0;i<4;i++)
	{
		jpegcodechandle = open(devname, O_RDWR);
		if (jpegcodechandle < 0)
		{			
			continue;	
		}
		break;
	}
    
	if (jpegcodechandle < 0)
	{
        printf("%s %d  Error JPEG Engine is busy\n",  __FUNCTION__,__LINE__);
		return 2; 
	}
    
	/* allocate memory for JPEG engine */
    int jpeg_buffer_size;
	ioctl(jpegcodechandle, JPEG_GET_JPEG_BUFFER, &jpeg_buffer_size);
	
	printf("\tjpeg engine memory buffer: 0x%X\n", jpeg_buffer_size);

	pJpegBuffer = mmap(NULL, jpeg_buffer_size, PROT_READ|PROT_WRITE, MAP_SHARED, jpegcodechandle, 0);
	if(pJpegBuffer == MAP_FAILED)
	{
        close(jpegcodechandle);
        printf("%s %d  Error JPEG Map Failed\n",  __FUNCTION__,__LINE__);
		return 3; 
	}

	unsigned int jpeg_phybuffer_addr;
	ioctl(jpegcodechandle, JPEG_GET_PHYPARAM, &jpeg_phybuffer_addr);

	printf("jpeg_phybuffer_addr = 0x%x\n", jpeg_phybuffer_addr);

    jpeg_info_t *pjpeginfo;
    pjpeginfo = malloc(sizeof(jpeg_info_t)+4);
    if(pjpeginfo == NULL)
	{
		printf("%s %d Error %s\n",__FUNCTION__,__LINE__,strerror(errno));
		return 5;
	}

    pjpegfilebuf = malloc(MAXJPEGFILELEN);
    if(pjpegfilebuf == NULL)
	{
		printf("%s %d Error %s\n",__FUNCTION__,__LINE__,strerror(errno));
		return 6;
	}

	pthread_mutex_t mutexhandle;
    rvalue = pthread_mutex_init(&mutexhandle, NULL);
    if(rvalue)
	{
        printf("%s %d  Error %s\n",  __FUNCTION__,__LINE__, strerror(errno));
		return 7;
	}

	pbaseinfo->mutexhandle = mutexhandle;
    pbaseinfo->jpegcodechandle = jpegcodechandle;
    
    pbaseinfo->mmapdatalen = jpeg_buffer_size;
    pbaseinfo->out_virt_buffer = pJpegBuffer;

    pbaseinfo->pjpeginfo = pjpeginfo;
    pbaseinfo->pjpegfilebuf = pjpegfilebuf;
    pbaseinfo->jpeg_phybuffer_addr = jpeg_phybuffer_addr;
    
    jpegdecoderflag = 1;
    return 0;
}

int jpegcodec_close_base()
{
    Jpeg_base_codec_info *pbaseinfo = &jpegbaseinfo;
    
    if(jpegdecoderflag)
    {   
    	pthread_mutex_destroy(&pbaseinfo->mutexhandle);
		
        if(pbaseinfo->pjpeginfo != NULL)
        {
            free(pbaseinfo->pjpeginfo);
            pbaseinfo->pjpeginfo = NULL;
        }

         if(pbaseinfo->pjpegfilebuf != NULL)
        {
            free(pbaseinfo->pjpegfilebuf);
            pbaseinfo->pjpegfilebuf = NULL;
        }

        if((void *)pbaseinfo->out_virt_buffer != NULL)
        {
             munmap((void *)pbaseinfo->out_virt_buffer, pbaseinfo->mmapdatalen); 
             pbaseinfo->out_virt_buffer = NULL;
        }
        
        if(pbaseinfo->jpegcodechandle > 0)
        {
            close(pbaseinfo->jpegcodechandle);
            pbaseinfo->jpegcodechandle = -1;
        }
        
        jpegdecoderflag = 0;
    }
    
    return 0;
}

//codec dec

static int deal_jpegdecodedata(unsigned char *psrcdatabuf, unsigned int datalen, unsigned int vaddr_dst, unsigned int paddr_dst, unsigned int stride)
{
    int rvalue;
    Jpeg_base_codec_info *pbaseinfo = &jpegbaseinfo;
    
    if(psrcdatabuf == NULL || jpegdecoderflag == 0)
        return 1;

    jpeg_param_t jpeg_param;
    memset(&jpeg_param, 0, sizeof(jpeg_param));
    
    jpeg_param.encode = 0;			                      
	jpeg_param.dst_bufsize = pbaseinfo->mmapdatalen-datalen;        
    jpeg_param.src_bufsize = datalen;	                    
    
	jpeg_param.decInWait_buffer_size = 0;	              /* Decode input Wait buffer size (Decode input wait function disable when 								   decInWait_buffer_size is 0) */
	jpeg_param.decopw_en = 0;
	jpeg_param.windec_en = 0;

    jpeg_param.buffersize = 0;		                      /* only for continuous shot */
   	jpeg_param.buffercount = 1;

	/* Set decode output format: RGB555/RGB565/RGB888/YUV422/PLANAR_YUV */
	jpeg_param.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_RGB565;		
  
    jpeg_param.scale = 0;		
    jpeg_param.scaled_width = -1;	

    jpeg_param.scaled_height = -1;	
	jpeg_param.dec_stride = stride/2;
            
    jpeg_param.paddr_dst = pbaseinfo->jpeg_phybuffer_addr + MAXJPEGFILELEN;
	jpeg_param.vaddr_dst = pbaseinfo->out_virt_buffer +  MAXJPEGFILELEN;
    
    memcpy((void*)pbaseinfo->out_virt_buffer, psrcdatabuf, datalen);

    if((ioctl(pbaseinfo->jpegcodechandle, JPEG_S_PARAM, &jpeg_param)) < 0)
	{
		return 1;
	}
  
    /* Trigger JPEG engine */
	if((ioctl(pbaseinfo->jpegcodechandle, JPEG_TRIGGER, NULL)) < 0)
	{
		printf("%s %d JPEG_TRIGGER error %s\n", __FUNCTION__,__LINE__, strerror(errno));
        return 4;
    }

	/* Get JPEG Encode information */
	rvalue = read(pbaseinfo->jpegcodechandle, pbaseinfo->pjpeginfo, sizeof(jpeg_info_t)+4);
	if(rvalue<0) 
    {
		printf("%s %d jpegdecoder_configparam error %d\n",__FUNCTION__,__LINE__, rvalue);
        return 5;
    }

	//printf("width = %d height = %d stride = %d \n", pbaseinfo->pjpeginfo->width, pbaseinfo->pjpeginfo->height, stride);

    if(pbaseinfo->pjpeginfo->state == JPEG_DECODED_IMAGE)
    {  
		int i;
		for(i=0;i<pbaseinfo->pjpeginfo->height; i++)
		{
			memcpy((void*)vaddr_dst+i*stride, (char*)(pbaseinfo->out_virt_buffer + MAXJPEGFILELEN+i*stride), pbaseinfo->pjpeginfo->width*2);
		}
        return 0;
    }

    return 0;
}


int DrawJpegControl(char* filepathname, int x,int y)
{
	int rvalue, filelen;
	FILE * infile;
	
	unsigned int paddr_dst, vaddr_dst, stride;
	Jpeg_base_codec_info *pbaseinfo = &jpegbaseinfo;
  	WinBaseBufInfo *pbufinfo = NULL;  

  	if(jpegdecoderflag == 0)
  	{
  		jpegcodec_open_base();
  		jpegdecoderflag = 1;
  	}
  	
	if((access(filepathname,F_OK))==-1)   
    {   
		printf("%s %d file not Exist %s  x=%d  y=%d \n",__FUNCTION__,__LINE__, filepathname,x,y);
		return -1;
    }
	
	pbufinfo = GetWinBaseBufInfo();
    if(pbufinfo == NULL || filepathname == NULL || jpegdecoderflag == 0)
        return -1;
    
	if ((infile = fopen(filepathname, "rb")) == NULL) 
    {
		printf("%s %d Can not open file %s \n",__FUNCTION__,__LINE__, filepathname);
		return -1;
	}

	pthread_mutex_lock(&pbaseinfo->mutexhandle);

	filelen = fread(pbaseinfo->pjpegfilebuf, 1, MAXJPEGFILELEN, infile);
    if(filelen <= 0)
    {   
    	pthread_mutex_unlock(&pbaseinfo->mutexhandle);
		printf("%s %d file not Exist %s  x=%d  y=%d \n",__FUNCTION__,__LINE__, filepathname,x,y);
		return -2;
    }
  	fclose(infile);

  	vaddr_dst =  (unsigned int)(pbufinfo->pdstbuf + y *pbufinfo->stride+x*2);
  	stride = pbufinfo->stride;
	paddr_dst = (unsigned int)(pbufinfo->dstpaddr + y *pbufinfo->stride+x*2);
	
  	rvalue = deal_jpegdecodedata(pbaseinfo->pjpegfilebuf, filelen, vaddr_dst, paddr_dst, stride);
  	if(rvalue)
    {   
    	pthread_mutex_unlock(&pbaseinfo->mutexhandle);
		printf("%s %d file not Exist %s  x=%d  y=%d \n",__FUNCTION__,__LINE__, filepathname,x,y);
		return -3;
    }

	pthread_mutex_unlock(&pbaseinfo->mutexhandle);
	return 0;
}


