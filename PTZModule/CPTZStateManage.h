#pragma once

enum enPTZ_State {
	PTZ_DEFAULT = -1, //默认的状态  --删除，不要这个
	PTZ_NO_OPEN = 0, //云台没有打开
	PTZ_SERIALPORT_CONNECT_SUCESS, //云台串口连接成功
	PTZ_SERIALPORT_CONNECT_FAIL,//云台串口连接失败
	PTZ_STOP, //云台没有在移动  --？不好控制？因为可能出现错乱的情况，_waitForNextCmd();  parseReadInfo()的两个地方
	PTZ_MOVE //云台正在移动
};
class CPTZStateManage
{
public:
	CPTZStateManage() {};
	~CPTZStateManage() {};

	enPTZ_State getPTZState() 
	{
		return m_curPTZState;
	}
	void setPTZState(enPTZ_State curPTZState)
	{
		m_curPTZState = curPTZState;
	}
private:
	enPTZ_State  m_curPTZState = PTZ_NO_OPEN;
};

