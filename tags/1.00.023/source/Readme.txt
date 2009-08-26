PDB Explorer自述文档

版本：v1.00.023(Unicode)
适用平台：Win2K/WinXP
版权所有：Titi工作室
作者：李马
电子邮件：titilima@163.com
作者主页：http://www.titilima.cn

简介
PDB Explorer是一个能够查看微软pdb文件（Program DataBase，程序数据库）的工具，它能够将pdb文件中的struct、union及enum类型的定义以C/C++的语法显示出来，特别适合Windows底层研究人员及DDK程序员使用。
请注意，在使用PDB Explorer之前，请确认您已经注册了微软的DIA库（msdia71.dll或msdia80.dll），否则PDB Explorer将不能运行。

FAQ
Q：如何自定义PDB Explorer的语法高亮色？
A：参照PDBExp.htm的样式编辑一个HTML文件，然后在“设置”中将您编辑好的HTML文件设为模板HTML文件，最后重新启动PDB Explorer即可。
Q：“整理符号”有什么用？
A：如果您在开发时要使用一个较大的结构（如_EPROCESS或_PEB），您可以使用“整理符号”功能选择对您有用的struct成员或enum项，PDB Explorer会将其它对您没用的项目以UCHAR（字节）数组的形式填充或省略。下面示范了经过整理的_EPROCESS结构（WinXP）：
    struct _EPROCESS // 0x260
    {
        UCHAR uUseless0[0x138]; // +0x0(0x138)
        void* SectionObject; // +0x138(0x4)
        UCHAR uUseless1[0x38]; // +0x13c(0x38)
        UCHAR ImageFileName[0x10]; // +0x174(0x10)
        UCHAR uUseless2[0x2c]; // +0x184(0x2c)
        struct _PEB* Peb; // +0x1b0(0x4)
        UCHAR uUseless3[0x94]; // +0x1b4(0x94)
        ULONG Flags; // +0x248(0x4)
        UCHAR uUseless4[0x18]; // +0x248(0x18)
    };
Q：struct成员后的注释是什么意思？
A：第一个数值是该成员相对struct的偏移，第二个数值是该成员的大小。
