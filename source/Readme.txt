PDB Explorer 自述文档

版本：v1.02.036(Unicode)
适用平台：Win2K/WinXP
版权所有：Titi工作室
作者：李马
电子邮件：titilima@163.com
作者主页：http://www.titilima.cn

简介
PDB Explorer 是一个能够查看微软 pdb 文件（Program DataBase ，程序数据库）的工具，它能够将 pdb 文件中的 struct 、union 及 enum 类型的定义以 C/C++ 的语法显示出来，特别适合 Windows 底层研究人员及 DDK 程序员使用。
请注意，在使用 PDB Explorer 之前，请确认您已经注册了微软的 DIA 库（msdia71.dll 或 msdia80.dll），否则 PDB Explorer 将不能运行。

FAQ
Q: 如何自定义 PDB Explorer 的语法高亮色？
A: 参照 PDBExp.htm 的样式编辑一个 HTML 文件，然后在“设置”中将您编辑好的 HTML 文件设为模板 HTML 文件，最后重新启动 PDB Explorer 即可。
Q: “整理符号”有什么用？
A: 如果您在开发时要使用一个较大的结构（如 _EPROCESS 或 _PEB），您可以使用“整理符号”功能选择对您有用的 struct 成员或 enum 项，PDB Explorer 会将其它对您没用的项目以 UCHAR （字节）数组的形式填充或省略。下面示范了经过整理的 _EPROCESS 结构（WinXP）：
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
Q: struct 成员后的注释是什么意思？
A: 第一个数值是该成员相对 struct 的偏移，第二个数值是该成员的大小。
