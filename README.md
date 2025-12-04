# test
2025_12_01
tcp服务器、客户端的完成，系统的框架

2025_12_02
UDP和UDP广播实现,可以进行json数据的解析和ini配置文件的解析，写入

2025_12_03
自动管理变量，修改后可直接修改iniReader_,修改完之后直接updateIniFile即可保存到配置文件中
可以根据启动程序时的传参来判断是否开启打印

2025_12_4
日志模块完成，可支持流输出，例如：LOG_INFO << "ver" << "stop";
DEBUG_PRINT开启打印到控制台
LOG_INFO-LOG_WARNING-LOG_ERROR
注意需要设置日志路径，和日志名，否则会默认输出路径和名字
/*
example:
    LOG::init(argc,argv);
    
    LOG::setLogPath(".");
    LOG::setLogName("test");
    LOG_INFO << "输出到文件" ;

    LOG::setDebugEnable(true);
    DEBUG_PRINT << "输出到控制台";
*/


/*注意：cmake版本最低 3.10*/
/*所有模块都在system中去启动和停止，统一由system管理*/
