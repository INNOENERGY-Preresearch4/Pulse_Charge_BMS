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


static u32 validLen_UART8Tx = 0;//��ʹ�õ����ݳ���
static u8* pHead_UART8Tx = NULL;//���δ洢�����׵�ַ 
static u8* pTail_UART8Tx = NULL;//���δ洢���Ľ�β��ַ 
static u8* pValid_UART8Tx = NULL;//��ʹ�õĻ��������׵�ַ (��ָ��)
static u8* pValidTail_UART8Tx = NULL;//��ʹ�õĻ�������β��ַ (дָ��)


static u32 validLen_UART8Rx = 0;//��ʹ�õ����ݳ���
static u8* pHead_UART8Rx = NULL;//���δ洢�����׵�ַ 
static u8* pTail_UART8Rx = NULL;//���δ洢���Ľ�β��ַ 
static u8* pValid_UART8Rx = NULL;//��ʹ�õĻ��������׵�ַ (��ָ��)
static u8* pValidTail_UART8Rx = NULL;//��ʹ�õĻ�������β��ַ (дָ��)


u8 UART8_Rev_RingBuf[Rev_RingBuf_LEN1];//UART8���ջ��λ�����
u8 UART8_Sen_RingBuf[Sen_RingBuf_LEN1];//UART8���ͻ��λ�����

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

//UART8Tx���ͻ��λ�����********************************************************************************
/*
 * ��ʼ�����λ�����
 * ���λ��������������malloc������ڴ�,Ҳ������Flash�洢����
 * */ 
void InitRingbuffer_UART8Tx(void) 
{ 
	pHead_UART8Tx=&UART8_Sen_RingBuf[0];
	pTail_UART8Tx=&UART8_Sen_RingBuf[Sen_RingBuf_LEN1-1];
	pValid_UART8Tx = pValidTail_UART8Tx = pHead_UART8Tx; 
	validLen_UART8Tx = 0; 
}


/*
 * function:�򻺳�����д������
 * param:@buffer д�������ָ��
 *       @addLen д������ݳ���
 * return:-1:д�볤�ȹ���
 *        -2:������û�г�ʼ��
 * */

int WirteRingbuffer_UART8Tx(u8* buffer,u32 addLen)
{
    if(addLen > Sen_RingBuf_LEN1) return -1;

	//��Ҫ���������copy��pValidTail_UART8Tx�� 
	if(pValidTail_UART8Tx + addLen > pTail_UART8Tx)//��Ҫ�ֳ�����copy 
	{ 
		int len1 = pTail_UART8Tx - pValidTail_UART8Tx; 
		int len2 = addLen - len1; 
		memcpy( pValidTail_UART8Tx, buffer, len1); 
		memcpy( pHead_UART8Tx, buffer + len1, len2); 
		pValidTail_UART8Tx = pHead_UART8Tx + len2;//�µ���Ч��������βָ�� 
			
	}
	else 
	{ 
		memcpy( pValidTail_UART8Tx, buffer, addLen); 
		pValidTail_UART8Tx += addLen;//�µ���Ч��������βָ�� 
	}
	//�����¼�����ʹ��������ʼλ�� 
	if(validLen_UART8Tx + addLen > Sen_RingBuf_LEN1) 
	{ 
		int moveLen = validLen_UART8Tx + addLen - Sen_RingBuf_LEN1;//��Чָ�뽫Ҫ�ƶ��ĳ��� 
		if(pValid_UART8Tx + moveLen > pTail_UART8Tx)//��Ҫ�ֳ����μ��� 
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
 * function:�ӻ�������ȡ������
 * param   :@buffer:���ܶ�ȡ���ݵ�buffer
 *          @len:��Ҫ��ȡ�����ݵĳ���
 * return  :-1:û�г�ʼ��
 *          >0:ʵ�ʶ�ȡ�ĳ���
 * */
		
int ReadRingbuffer_UART8Tx(u8* buffer,u32 len) 
{ 
	if(validLen_UART8Tx ==0) return 0; 
	if( len > validLen_UART8Tx) 
		len = validLen_UART8Tx; 
	if(pValid_UART8Tx + len > pTail_UART8Tx)//��Ҫ�ֳ�����copy 
	{ 
		int len1 = pTail_UART8Tx - pValid_UART8Tx; 
		int len2 = len - len1; 
		memcpy( buffer, pValid_UART8Tx, len1);//��һ�� 
		memcpy( buffer+len1, pHead_UART8Tx, len2);//�ڶ��Σ��Ƶ������洢���Ŀ�ͷ 
		pValid_UART8Tx = pHead_UART8Tx + len2;//������ʹ�û���������ʼ 
	}
	else 
	{ 
		memcpy( buffer, pValid_UART8Tx, len); 
		pValid_UART8Tx = pValid_UART8Tx +len;//������ʹ�û���������ʼ 
	} 
	validLen_UART8Tx -= len;//������ʹ�û������ĳ��� 
	return len; 
}

/*
 * function:��ȡ��ʹ�û������ĳ���
 * return  :��ʹ�õ�buffer����
 * */
u32 GetRingbufferValidLen_UART8Tx(void)
{
    return validLen_UART8Tx;
}





//UART8Rx���ջ��λ�����********************************************************************************
/*
 * ��ʼ�����λ�����
 * ���λ��������������malloc������ڴ�,Ҳ������Flash�洢����
 * */ 
void InitRingbuffer_UART8Rx(void) 
{ 
	pHead_UART8Rx=&UART8_Rev_RingBuf[0];
	pTail_UART8Rx=&UART8_Rev_RingBuf[Rev_RingBuf_LEN1-1];
	pValid_UART8Rx = pValidTail_UART8Rx = pHead_UART8Rx; 
	validLen_UART8Rx = 0; 
}


/*
 * function:�򻺳�����д������
 * param:@buffer д�������ָ��
 *       @addLen д������ݳ���
 * return:-1:д�볤�ȹ���
 *        -2:������û�г�ʼ��
 * */

int WirteRingbuffer_UART8Rx(u8* buffer,u32 addLen)
{
    if(addLen > Rev_RingBuf_LEN1) return -1;

	//��Ҫ���������copy��pValidTail_UART8Rx�� 
	if(pValidTail_UART8Rx + addLen > pTail_UART8Rx)//��Ҫ�ֳ�����copy 
	{ 
		int len1 = pTail_UART8Rx - pValidTail_UART8Rx; 
		int len2 = addLen - len1; 
		memcpy( pValidTail_UART8Rx, buffer, len1); 
		memcpy( pHead_UART8Rx, buffer + len1, len2); 
		pValidTail_UART8Rx = pHead_UART8Rx + len2;//�µ���Ч��������βָ�� 
			
	}
	else 
	{ 
		memcpy( pValidTail_UART8Rx, buffer, addLen); 
		pValidTail_UART8Rx += addLen;//�µ���Ч��������βָ�� 
	}
	//�����¼�����ʹ��������ʼλ�� 
	if(validLen_UART8Rx + addLen > Rev_RingBuf_LEN1) 
	{ 
		int moveLen = validLen_UART8Rx + addLen - Rev_RingBuf_LEN1;//��Чָ�뽫Ҫ�ƶ��ĳ��� 
		if(pValid_UART8Rx + moveLen > pTail_UART8Rx)//��Ҫ�ֳ����μ��� 
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
 * function:�ӻ�������ȡ������
 * param   :@buffer:���ܶ�ȡ���ݵ�buffer
 *          @len:��Ҫ��ȡ�����ݵĳ���
 * return  :-1:û�г�ʼ��
 *          >0:ʵ�ʶ�ȡ�ĳ���
 * */
		
int ReadRingbuffer_UART8Rx(u8* buffer,u32 len) 
{ 
	if(validLen_UART8Rx ==0) return 0; 
	if( len > validLen_UART8Rx) 
		len = validLen_UART8Rx; 
	if(pValid_UART8Rx + len > pTail_UART8Rx)//��Ҫ�ֳ�����copy 
	{ 
		int len1 = pTail_UART8Rx - pValid_UART8Rx; 
		int len2 = len - len1; 
		memcpy( buffer, pValid_UART8Rx, len1);//��һ�� 
		memcpy( buffer+len1, pHead_UART8Rx, len2);//�ڶ��Σ��Ƶ������洢���Ŀ�ͷ 
		pValid_UART8Rx = pHead_UART8Rx + len2;//������ʹ�û���������ʼ 
	}
	else 
	{ 
		memcpy( buffer, pValid_UART8Rx, len); 
		pValid_UART8Rx = pValid_UART8Rx +len;//������ʹ�û���������ʼ 
	} 
	validLen_UART8Rx -= len;//������ʹ�û������ĳ��� 
	return len; 
}

/*
 * function:��ȡ��ʹ�û������ĳ���
 * return  :��ʹ�õ�buffer����
 * */
u32 GetRingbufferValidLen_UART8Rx(void)
{
    return validLen_UART8Rx;
}



