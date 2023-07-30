/********************************************************************/
//	作用：				STM32串口数据发送与接收
//	作者：				FITQY
//	时间：				2022.08.18
/********************************************************************/
#include "uartdata.h"
#include "system.h"

/********************************************************************

串口发送数据结构体初始化函数 Data_Init

	 各参数作用
	 DataTransmit *data: 							    选择要初始化的结构体 如 &data_uart2
	 head1：			        						帧头1 如 0xAA
	 head2：			        						帧头2 如 0xAA
	 length：			        						有效数据长度 
	 
********************************************************************/
void Data_Init_Transmit(DataTransmit *data, uint8_t head1, uint8_t head2, uint8_t length)
{
	
	data -> head1  = head1;
	data -> head2  = head2;
	data -> length = length;
	
	for(uint8_t i = 0; i < length; i++)
	{
		data -> data[i] = 0;
	}
	
	for(uint8_t j = 0; j < length + 4; j++)
	{
		data -> transmit_data[j] = 0;
	}
	
}

/********************************************************************

串口发送数据打包函数 Data_Pack

	 各参数作用
	 DataTransmit *data: 			带入结构体 如 &data_uart2
	 
********************************************************************/
void Data_Pack(DataTransmit *data)
{
	
	data -> transmit_data[0] = data -> head1;  //帧头1
	data -> transmit_data[1] = data -> head2;  //帧头2
	data -> transmit_data[2] = data -> length; //有效数据长度
	
	for(uint8_t i = 0; i < data -> length; i++)
	{
		data -> transmit_data[3+i] = data -> data[i];
	}
	
	uint8_t sum = 0;
	
  for(uint8_t j = 0; j < data -> length + 3; j++)
	{
		sum += data -> transmit_data[j];
	}
	
  data -> transmit_data[data -> length + 3] = sum;
	
}

/********************************************************************

串口数据发送函数 Data_Transmit

	 各参数作用
	 UART_HandleTypeDef *huart:				选择通过哪一个串口发送 如 &huart2
	 DataTransmit *data: 	        		带入结构体 如 &data_uart2
	
	注意：此处用到了 STM32 HAL库 的函数 HAL_UART_Transmit 若非 HAL库 版本的代码 无法使用 
	 
********************************************************************/
void Data_Transmit(UART_HandleTypeDef *huart, DataTransmit *data)
{
	
	HAL_UART_Transmit(huart, data -> transmit_data, data -> transmit_data[2] + 4 , 0xFFFF);
	
}

/********************************************************************

串口数据打包发送函数 Data_Pack_Transmit

	 各参数作用
	 UART_HandleTypeDef *huart:				选择通过哪一个串口发送 如 &huart2
	 DataTransmit *data: 	        		带入结构体 如 &data_uart2
	 
********************************************************************/
void Data_Pack_Transmit(UART_HandleTypeDef *huart, DataTransmit *data)
{
	
  Data_Pack(data);				
  Data_Transmit(huart,data);			
	
}

// 至此串口数据发送函数定义结束 使用例如下
/*

DataTransmit data_uart2;						// 声明全局结构体 data_uart2 这个要放在 main 函数外面

Data_Init_Transmit(&data_uart2,0xAA,0xAA,1);	// main函数中在while(1)前 对结构体 data_uart2 进行初始化 这里的长度是有效数据长度 比如发送1个模式控制位 就带入 1

data_uart2.data[0] = 1;							// 模式控制位 为1 这段放的位置依据个人需求 需要更改 K210 或者 OpenMV 工作模式 则将 data_uart2.data[0] 改成需要的数即可 这里的 1 代表 0x01

while(1)										// 在while(1)中 或者依据个人需求 放在其他函数中
{
	Data_Pack_Transmit(&huart2,&data_uart2);	// 对数据进行打包发送
}

*/
// 至此串口数据发送函数使用例结束 接下来完成对数据接收函数定义

/********************************************************************

串口接收数据结构体初始化函数 Data_Init_Receive

	 各参数作用
	 DataReceive *data: 							    选择要初始化的结构体 如 &data_uart2_receive
	 head1：			        						帧头1 如 0xAA
	 head2：			        						帧头2 如 0xAA
	 
********************************************************************/
void Data_Init_Receive(DataReceive *data, uint8_t head1, uint8_t head2)
{
	
	data -> head1  = head1;
	data -> head2  = head2;
	data -> length = 0;
	data -> i      = 0;
	data -> cnt    = 0;
	data -> state  = 0;
	
	for(uint8_t j = 0; j < 50; j++)
	{
		data -> receive_data[j] = 0;
	}
	
}

/********************************************************************

串口接收1个数据函数 Buf_Receive

	 各参数作用
	 UART_HandleTypeDef *huart:			选择通过哪一个串口接收 如 &huart2
	 DataReceive *data: 				选择要带入的结构体 如 &data_uart2_receive
	 
********************************************************************/
uint8_t Buffer_Receive(UART_HandleTypeDef *huart, DataReceive *data)
{
	HAL_UART_Receive_IT(huart, &data -> data, 1);
	return data -> data;
}

/********************************************************************

串口接收数据函数 Data_Receive

	 各参数作用
	 DataReceive *data: 						选择要带入的结构体 如 &data_uart2_receive
	 buf:										串口接收到的数据
	 
********************************************************************/
void Data_Receive(DataReceive *data, uint8_t buf)
{
	
	if(data -> state == 0 && buf == data -> head1)
	{
		
		data -> state = 1;
		data -> receive_data[0] = buf;
		
	}
	else if(data -> state == 1 && buf == data -> head2)
	{
		
		data -> state = 2;
		data -> receive_data[1] = buf;
		
	}
	else if(data -> state == 2 && buf < 40)
	{
		
		data -> state = 3;
		data -> length = buf;
		data -> cnt = buf+4;
		data -> receive_data[2] = buf;
		
	}
	else if(data -> state == 3 && data -> length > 0)
	{
		
		data -> length = data -> length - 1;
		data -> receive_data[3 + data -> i] = buf;
		data -> i = data -> i + 1;
		if(data -> length == 0)
		{
			
			data -> state = 4;
			
		}
		
	}
	else if(data -> state == 4)
	{
		
		data -> receive_data[3 + data -> i] = buf;
		data -> state = 0;
		data -> i = 0;
		
	}
	else
	{
		
		data -> state = 0;
		data -> i = 0;
		
	}
	
}

// 至此串口数据接收函数定义结束 使用例如下
/*

DataReceive data_uart2_receive;										// 声明全局结构体 data_uart2_receive 这个要放在 main 函数外面

Data_Init_Receive(&data_uart2_receive,0xAA,0xAA);					// main的函数在while(1)前 对接收结构体进行初始化

Buffer_Receive(&huart2,&data_uart2_receive);						// main的函数在while(1)前 HAL库需要接收一次 使之可以进入中断回调

// 中断回调函数中调用
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if( huart == &huart2 )											// 检测到串口中断2
	{

		Data_Receive(&data_uart2_receive,Buffer_Receive(&huart2,&data_uart2_receive));

	}

}

// 需注意 函数 HAL_UART_RxCpltCallback 是一个弱定义 可以在 main.c 的 USER CODE BEGIN 4 中进行重写

*/
// 至此串口数据接收函数使用例结束 接下来举例说明数据解析

/********************************************************************

串口接收数据结构体初始化函数 Target_Init

	 各参数作用
	 TargetAttribute *target: 	选择要初始化的结构体 如 &target_1
	 
********************************************************************/
void Target_Init(TargetAttribute *target)
{
	
	target -> x     = 0;
	target -> y     = 0;
	target -> color = 0;
	target -> shape = 0;
	target -> flag  = 0;
	
}

/********************************************************************

串口接收数据结构体初始化函数 Target_Init

   各参数作用
   DataReceive *data: 				选择要带入的结构体 如 &data_uart2_receive
   TargetAttribute *target: 		选择要带入的结构体 如 &target_1
	 
********************************************************************/
void Target_Parse(DataReceive *data,TargetAttribute *target)
{
	
	uint8_t sum = 0;
	uint8_t i   = 0;
	
	while(i < data -> cnt - 1)
	{
		
		sum = sum + data -> receive_data[i];
		i = i + 1;
		
	}
	
	if(sum == data -> receive_data[data -> cnt - 1])
	{
		
		target -> x     = data -> receive_data[3]*256 + data -> receive_data[4];
		target -> y     = data -> receive_data[5]*256 + data -> receive_data[6];
		target -> color = data -> receive_data[7];
		target -> shape = data -> receive_data[8];
		target -> flag  = data -> receive_data[9];
		
	}
	
}

// 至此数据解析函数定义结束 使用例如下
/*

TargetAttribute target_1;											// 声明全局结构体 target_1 这个要放在 main 函数外面

Target_Init(&target_1);												// main的函数在while(1)前 对解析结构体进行初始化

Target_Parse(&data_uart2_receive,&target_1);						// 可以放在while(1)中 也可自定义放在其他地方

*/
// 解析函数可根据自身需求自由定义 方法不唯一

