# SKX-LLRB
Programming Practice LLRB
一个左倾红黑树编程练习，编写了一个模版类，可以改为.h使用

The program is a template for LLRB.It can change to '.h' file to use.

LLRB<type> A;

外部接口

.in(T key);  operator+

.out(T key);  operator-

.search(T key)

.CheckBalance(); Check balance to search error and return tree deepth

.showTree(); use cout<< to show tree element sequentially//顺序遍历树

.Qs();show tree(parent and child)and to check err//显示树结构，内部可以填写需要查看的根节点用于查找错误


左倾红黑树的编写练习过程遇到了不少困难，我没有在网上找到代码示例，所以全部安装流程示意图编写，特别是删除过程，可能会比较低效

我看的删除示例是一种流动方案，让红色叶节点使用流动方法挤出删除节点

我的思路：检查红色叶节点，若没有首先把红色树枝降降级，暂时挤出一个节点，完成删除后再插入，树高坍塌也是此过程

递归方法查找待删除节点和红色叶节点，将流动路径压入双向链表，然后流动挤出，删除红色叶节点

全程使用指针操作，唯一的复制是载入关键字的过程，为了减少使用string类等的操作速度，如果传入结构体和类，至少应该重载 三个比较运算，赋值，复制运算，信息流运算可选


这只是我编程学习中的一个练习项目，还有很多改进空间。
