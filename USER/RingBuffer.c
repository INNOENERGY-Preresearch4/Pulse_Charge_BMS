/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
*                                       Keil Development Kits
*                                              on the
*
*                                              Terminus
*
* Filename      : RingBuffer.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "RingBuffer.h"

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


static u32 validLen_UART8Tx = 0;//已使用的数据长度
static u8* pHead_UART8Tx = NULL;//环形存储区的首地址 
static u8* pTail_UART8Tx = NULL;//环形存储区的结尾地址 
static u8* pValid_UART8Tx = NULL;//已使用的缓冲区的首地址 (读指针)
static u8* pValidTail_UART8Tx = NULL;//已使用的缓冲区的尾地址 (写指针)


static u32 validLen_UART8Rx = 0;//已使用的数据长度
static u8* pHead_UART8Rx = NULL;//环形存储区的首地址 
static u8* pTail_UART8Rx = NULL;//环形存储区的结尾地址 
static u8* pValid_UART8Rx = NULL;//已使用的缓冲区的首地址 (读指针)
static u8* pValidTail_UART8Rx = NULL;//已使用的缓冲区的尾地址 (写指针)


u8 UART8_Rev_RingBuf[Rev_RingBuf_LEN1];//UART8接收环形缓存区
u8 UART8_Sen_RingBuf[Sen_RingBuf_LEN1];//UART8发送环形缓存区

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

//UART8Tx发送环形缓冲区********************************************************************************
/*
 * 初始化环形缓冲区
 * 环形缓冲区这里可以是malloc申请的内存,也可以是Flash存储介质
 * */ 
void InitRingbuffer_UART8Tx(void) 
{ 
	pHead_UART8Tx=&UART8_Sen_RingBuf[0];
	pTail_UART8Tx=&UART8_Sen_RingBuf[Sen_RingBuf_LEN1-1];
	pValid_UART8Tx = pValidTail_UART8Tx = pHead_UART8Tx; 
	validLen_UART8Tx = 0; 
}


/*
 * function:向缓冲区中写入数据
 * param:@buffer 写入的数据指针
 *       @addLen 写入的数据长度
 * return:-1:写入长度过大
 *        -2:缓冲区没有初始化
 * */

int WirteRingbuffer_UART8Tx(u8* buffer,u32 addLen)
{
    if(addLen > Sen_RingBuf_LEN1) return -1;

	//将要存入的数据copy到pValidTail_UART8Tx处 
	if(pValidTail_UART8Tx + addLen > pTail_UART8Tx)//需要分成两段copy 
	{ 
		int len1 = pTail_UART8Tx - pValidTail_UART8Tx; 
		int len2 = addLen - len1; 
		memcpy( pValidTail_UART8Tx, buffer, len1); 
		memcpy( pHead_UART8Tx, buffer + len1, len2); 
		pValidTail_UART8Tx = pHead_UART8Tx + len2;//新的有效数据区结尾指针 
			
	}
	else 
	{ 
		memcpy( pValidTail_UART8Tx, buffer, addLen); 
		pValidTail_UART8Tx += addLen;//新的有效数据区结尾指针 
	}
	//需重新计算已使用区的起始位置 
	if(validLen_UART8Tx + addLen > Sen_RingBuf_LEN1) 
	{ 
		int moveLen = validLen_UART8Tx + addLen - Sen_RingBuf_LEN1;//有效指针将要移动的长度 
		if(pValid_UART8Tx + moveLen > pTail_UART8Tx)//需要分成两段计算 
		{ 
			int len1 = pTail_UART8Tx - pValid_UART8Tx; 
			int len2 = moveLen - len1; 
			pValid_UART8Tx = pHead_UART8Tx + len2; 
		}
		else 
		{ 
			pValid_UART8Tx = pValid_UART8Tx + moveLen; 
		} 
		validLen_UART8Tx = Sen_RingBuf_LEN1; 
	}
	else 
	{ 
		validLen_UART8Tx += addLen; 
	} 
	return 0; 
}

/*
 * function:从缓冲区内取出数据
 * param   :@buffer:接受读取数据的buffer
 *          @len:将要读取的数据的长度
 * return  :-1:没有初始化
 *          >0:实际读取的长度
 * */
		
int ReadRingbuffer_UART8Tx(u8* buffer,u32 len) 
{ 
	if(validLen_UART8Tx ==0) return 0; 
	if( len > validLen_UART8Tx) 
		len = validLen_UART8Tx; 
	if(pValid_UART8Tx + len > pTail_UART8Tx)//需要分成两段copy 
	{ 
		int len1 = pTail_UART8Tx - pValid_UART8Tx; 
		int len2 = len - len1; 
		memcpy( buffer, pValid_UART8Tx, len1);//第一段 
		memcpy( buffer+len1, pHead_UART8Tx, len2);//第二段，绕到整个存储区的开头 
		pValid_UART8Tx = pHead_UART8Tx + len2;//更新已使用缓冲区的起始 
	}
	else 
	{ 
		memcpy( buffer, pValid_UART8Tx, len); 
		pValid_UART8Tx = pValid_UART8Tx +len;//更新已使用缓冲区的起始 
	} 
	validLen_UART8Tx -= len;//更新已使用缓冲区的长度 
	return len; 
}

/*
 * function:获取已使用缓冲区的长度
 * return  :已使用的buffer长度
 * */
u32 GetRingbufferValidLen_UART8Tx(void)
{
    return validLen_UART8Tx;
}





//UART8Rx接收环形缓冲区********************************************************************************
/*
 * 初始化环形缓冲区
 * 环形缓冲区这里可以是malloc申请的内存,也可以是Flash存储介质
 * */ 
void InitRingbuffer_UART8Rx(void) 
{ 
	pHead_UART8Rx=&UART8_Rev_RingBuf[0];
	pTail_UART8Rx=&UART8_Rev_RingBuf[Rev_RingBuf_LEN1-1];
	pValid_UART8Rx = pValidTail_UART8Rx = pHead_UART8Rx; 
	validLen_UART8Rx = 0; 
}


/*
 * function:向缓冲区中写入数据
 * param:@buffer 写入的数据指针
 *       @addLen 写入的数据长度
 * return:-1:写入长度过大
 *        -2:缓冲区没有初始化
 * */

int WirteRingbuffer_UART8Rx(u8* buffer,u32 addLen)
{
    if(addLen > Rev_RingBuf_LEN1) return -1;

	//将要存入的数据copy到pValidTail_UART8Rx处 
	if(pValidTail_UART8Rx + addLen > pTail_UART8Rx)//需要分成两段copy 
	{ 
		int len1 = pTail_UART8Rx - pValidTail_UART8Rx; 
		int len2 = addLen - len1; 
		memcpy( pValidTail_UART8Rx, buffer, len1); 
		memcpy( pHead_UART8Rx, buffer + len1, len2); 
		pValidTail_UART8Rx = pHead_UART8Rx + len2;//新的有效数据区结尾指针 
			
	}
	else 
	{ 
		memcpy( pValidTail_UART8Rx, buffer, addLen); 
		pValidTail_UART8Rx += addLen;//新的有效数据区结尾指针 
	}
	//需重新计算已使用区的起始位置 
	if(validLen_UART8Rx + addLen > Rev_RingBuf_LEN1) 
	{ 
		int moveLen = validLen_UART8Rx + addLen - Rev_RingBuf_LEN1;//有效指针将要移动的长度 
		if(pValid_UART8Rx + moveLen > pTail_UART8Rx)//需要分成两段计算 
		{ 
			int len1 = pTail_UART8Rx - pValid_UART8Rx; 
			int len2 = moveLen - len1; 
			pValid_UART8Rx = pHead_UART8Rx + len2; 
		}
		else 
		{ 
			pValid_UART8Rx = pValid_UART8Rx + moveLen; 
		} 
		validLen_UART8Rx = Rev_RingBuf_LEN1; 
	}
	else 
	{ 
		validLen_UART8Rx += addLen; 
	} 
	return 0; 
}

/*
 * function:从缓冲区内取出数据
 * param   :@buffer:接受读取数据的buffer
 *          @len:将要读取的数据的长度
 * return  :-1:没有初始化
 *          >0:实际读取的长度
 * */
		
int ReadRingbuffer_UART8Rx(u8* buffer,u32 len) 
{ 
	if(validLen_UART8Rx ==0) return 0; 
	if( len > validLen_UART8Rx) 
		len = validLen_UART8Rx; 
	if(pValid_UART8Rx + len > pTail_UART8Rx)//需要分成两段copy 
	{ 
		int len1 = pTail_UART8Rx - pValid_UART8Rx; 
		int len2 = len - len1; 
		memcpy( buffer, pValid_UART8Rx, len1);//第一段 
		memcpy( buffer+len1, pHead_UART8Rx, len2);//第二段，绕到整个存储区的开头 
		pValid_UART8Rx = pHead_UART8Rx + len2;//更新已使用缓冲区的起始 
	}
	else 
	{ 
		memcpy( buffer, pValid_UART8Rx, len); 
		pValid_UART8Rx = pValid_UART8Rx +len;//更新已使用缓冲区的起始 
	} 
	validLen_UART8Rx -= len;//更新已使用缓冲区的长度 
	return len; 
}

/*
 * function:获取已使用缓冲区的长度
 * return  :已使用的buffer长度
 * */
u32 GetRingbufferValidLen_UART8Rx(void)
{
    return validLen_UART8Rx;
}



