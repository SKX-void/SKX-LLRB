//LLRB<T>5.1
//@author SKX
#ifndef SKX_LLRB
#define SKX_LLRB
//以前版本存在内存泄漏和非法delete,请勿使用
#include<iostream>//Qs接口和showTree接口需要调用std::cout和std::string,可以删除
#include <string>
#include<sstream>//Qs改写成ostringstream方法
#include<fstream>//所以包含了两个流头文件
//如果自定义结构体传入,需要重载比较,复制,赋值,数据流<<(可选)运算符
template<typename T = int>
class LLRB {
private:
	//内部数据结构
	enum { red = 0, black = 1 };
	struct Tree
	{
		bool color = red;
		T* key = 0x0;
		Tree* l = 0x0, * r = 0x0;
	};
	Tree* root;
	//操作组件模块
	//构造
	bool ClearTree(Tree* node, Tree* last);
	//查找组件
	bool do_SearchNode(const T& key, Tree* root, Tree*& get_p_of_key, Tree*& get_last_of_p);
	//平衡器
	int toBalance(Tree*& p);
	bool L_Rotate(Tree*& node);
	bool R_Rotate(Tree*& node);
	//插入
	int do_in(const T& key, Tree*& p);
	bool CheckRoot();
	//删除
	struct Queue
	{
		Tree* t = 0x0;
		Queue* last = 0x0, * next = 0x0;
	};
	int nodeOut(Tree* p, const T& key);
	int do_getQueue(int const& target, Tree* p, Tree* last, Tree*& redlast, Queue*& q, bool& tar, bool& gred);
	bool checkred(Tree* p, Tree*& redBranch, Tree*& last, Tree* inlast);
	bool downRed(Tree*& redBranch, const T& key, Queue*& q, bool& finish, Tree* last);
	bool do_R_downRed(Tree*& p, bool& out, Tree*& outnode, Tree* last);
	bool do_L_downRed(Tree*& p, bool& out, Tree*& outnode, Tree* last);

	//检查点组件
	bool getBlance(Tree* p, int deep, int lastdeep);
	bool do_show(Tree* root);
	//可选功能
	class showQue {
		struct Que {
			int deep;
			std::string show;
			Que* next;
		};
		Que* root;
	public:
		showQue() { root = new Que; root->deep = 0; root->show = ""; root->next = 0x0; }
		~showQue() { Que* p = root; while (p) { p = root->next; delete root; root = p; } }
		bool in(int _deep, std::string _show) {
			Que* p = root, * last = 0x0;
			while (1) {
				if (p->deep != _deep) { last = p; p = p->next; }
				else { p->show += _show; return true; }
				if (p == 0x0) { p = last; p->next = new Que; p->next->deep = (p->deep + 1); p = p->next; p->next = 0x0; }
			}
		}
		void show() { Que* p = root; while (p) { std::cout << p->show << '\n'; p = p->next; } }
	};
	void Que(Tree* p, showQue& s, int deep);
	//设置非法操作
	LLRB(const LLRB&){}//复制
	LLRB& operator=(const LLRB&){}//赋值
public:
	//构造组件
	LLRB<T>(const T& key);
	LLRB<T>();
	~LLRB<T>();
	//操作组件
	bool search(const T& key);
	int in(const T& key);
	int out(const T& key);
	//检查组件
	int CheckBalance();//检查树的平衡性,同时返回树高(一枝黑节点个数)
	bool showTree();//顺序遍历树
	///可选功能
	void Qs(Tree* p = 0x0);//展示树的结构,需要showQue类
	//重载组件
	LLRB<T>& operator+(const T& key);
	LLRB<T>& operator-(const T& key);
};
//构造
template<typename T>
LLRB<T>::LLRB<T>(const T& key) { root = new Tree; root->key = new T; *root->key = key; }
template<typename T>
LLRB<T>::LLRB<T>() { root = 0x0; }
template<typename T>
LLRB<T>::~LLRB<T>() { ClearTree(root, root); }
template<typename T>
bool LLRB<T>::ClearTree(Tree* node, Tree* last) {
	if (!node)return true;
	if (root->l != 0x0)ClearTree(node->l, node);
	if (root->r != 0x0)ClearTree(node->r, node);
	if (*node->key > *last->key)last->r = 0x0; else last->l = 0x0;
	delete node->key;
	delete node;
	return true;
}

//公用工具
template<typename T>
bool LLRB<T>::search(const T& key) {
	Tree* last = 0x0, * p = 0x0;
	return do_SearchNode(key, root, p, last);
}
template<typename T>
bool LLRB<T>::do_SearchNode(const T& key, Tree* root, Tree*& p, Tree*& last)
{
	if (!root)return false;
	last = p; p = root;
	if (*root->key == key) return true;
	if (*root->key > key)return do_SearchNode(key, root->l, p, last);
	else return do_SearchNode(key, root->r, p, last);
}

//平衡器
template<typename T>
int LLRB<T>::toBalance(Tree*& p) {
	if (!p)return -1;
	bool redo = true;
	while (redo) {
		redo = false;
		if (p->r)//为了防止对空指针进行操作
			if (!p->r->color) { L_Rotate(p); redo = true; }
		if (p->l) {
			if (p->l->l)
				if (!p->l->color && !p->l->l->color) { R_Rotate(p); redo = true; }
			if (p->r)
				if (!p->l->color && !p->r->color) {
					p->l->color = p->r->color = p->color;
					p->color = red;
					redo = true;
				}
		}
	}
	CheckRoot();
	return 1;
}
template<typename T>
bool LLRB<T>::L_Rotate(Tree*& node)
{
	if (!node || !node->r)return false;
	Tree* p = node, * q = p->r, * s = q->l;
	if (p->color != q->color) { p->color = !p->color; q->color = !q->color; }
	q->l = p; p->r = s;	node = q;
	return true;
}
template<typename T>
bool LLRB<T>::R_Rotate(Tree*& node)
{
	if (!node || !node->l)return false;
	Tree* p = node, * q = p->l, * s = q->r;
	if (p->color != q->color) { p->color = !p->color; q->color = !q->color; }
	q->r = p; p->l = s; node = q;
	return true;
}

//插入
template<typename T>
int LLRB<T>::in(const T& key) {
	if (root == 0x0) {
		root = new Tree; root->key = new T; *root->key = key;
		return 1;
	}
	do_in(key, root);
	CheckRoot();
	return 1;
}
template<typename T>
int LLRB<T>::do_in(const T& key, Tree*& p) {
	if (!p) {
		Tree* newNode = new Tree; newNode->key = new T;
		*newNode->key = key;
		p = newNode;
		return 1;
	}
	if (key == *p->key)return -1;
	int i = 0;
	if (key < *p->key) { i = do_in(key, p->l); }
	else { i = do_in(key, p->r); }
	toBalance(p);
	return i;
}
template<typename T>
bool LLRB<T>::CheckRoot() {
	if (root->color == black)return true;
	else root->color = black; return false;
}

//删除
template<typename T>
int LLRB<T>::out(const T& key) {
	Tree* p = 0x0, * last = 0x0;
	if (!do_SearchNode(key, root, p, last)) { return -1; }//未找到
	if (!p->color)//红叶
		if (!p->l) { delete p->key; delete p; last->l = 0x0; return 1; }
	nodeOut(p, key);
	return 1;
}
template<typename T>
int LLRB<T>::nodeOut(Tree* p, const T& key) {
	if (key == *root->key && !root->r) {
		if (p->l) { delete root->key; root->key = root->l->key; delete root->l; root->l = 0x0; }
		else { delete root->key; delete root; root = 0x0; }
		return 1;
	}//特殊情况,根,节点<3
	bool hasOut = false, finish = false, change = false;//左右情况
	Queue* que = 0x0;//流动队列
	Tree* redBranch = 0x0, * last = 0x0;
	if (!checkred(root, redBranch, last, 0x0))
		if (redBranch) {
			downRed(redBranch, key, que, finish, last);
			if (finish)return 1;//下降红节点过程已经完成out操作
			hasOut = true;
		}
		else {
			do_R_downRed(root, hasOut, redBranch, 0x0);
			if (key == *redBranch->key) return 1;
			if (key == *root->key && !root->r) {
				if (p->l) { delete root->key; root->key = root->l->key; delete root->l; root->l = 0x0; }
				else { delete root->key; delete root; root = 0x0; }
				if (hasOut)	in(*redBranch->key);
				return 1;
			}
			hasOut = true;
		}
	Tree* redlast = 0x0;
	bool tar = false, gred = false;//递归开关
	if (p->key)	do_getQueue(key, root, 0x0, redlast, que, tar, gred);
	if (que) {
		Queue* head = que, * tail = que;
		if (!que->next)while (head->last) { head = head->last; }
		if (!que->last)while (tail->last) { tail = tail->last; }
		if (key == *head->t->key)change = true;
		else if (key == *tail->t->key)change = false;
		else throw"lost_key_node";
	}
	if (change) {
		while (que->last) { que = que->last; }
		delete que->t->key;
		while (que->next) { que->t->key = que->next->t->key; que = que->next; }
	}
	else {
		delete que->t->key;
		while (que->last) { que->t->key = que->last->t->key; que = que->last; }
	}
	if (que)
		if (que->last)while (que->last) { que = que->last; delete que->next; }
		else while (que->next) { que = que->next; delete que->last; }
	delete que;
	delete redlast->l; 	redlast->l = 0x0;//减少一个节点
	if (hasOut) {
		in(*redBranch->key);
		delete redBranch->key;
		delete redBranch;
	}
	return 1;
}
template<typename T>
int LLRB<T>::do_getQueue(const int& target, Tree* p, Tree* last, Tree*& redlast, Queue*& q, bool& tar, bool& gred) {
	if (!p)return 0;
	if (tar && gred)return 1;
	do_getQueue(target, p->l, p, redlast, q, tar, gred);
	if (tar && gred)return 1;
	if (!p->color && !p->l)
		if (gred) {
			redlast = last;
			Queue* temp = 0x0;
			while (q) {
				temp = q;
				q = q->last;
				delete temp;
			}
		}
		else {
			gred = true;
			redlast = last;
		}
	if (*p->key == target)tar = true;
	if (gred || tar) {
		Queue* nq = new Queue; nq->t = p;
		if (q) { nq->last = q; q->next = nq; q = q->next; }
		else { q = nq; }
	}
	do_getQueue(target, p->r, p, redlast, q, tar, gred);
	if (tar && gred)return 1;
}
template<typename T>
bool LLRB<T>::checkred(Tree* p, Tree*& redBranch, Tree*& last, Tree* inlast) {
	if (!p)return false;
	if (!p->color && !p->l)return true;
	if (!p->color) { redBranch = p; last = inlast; }
	if (checkred(p->l, redBranch, last, p))return true;
	if (checkred(p->r, redBranch, last, p))return true;
	return false;
}
template<typename T>
bool LLRB<T>::downRed(Tree*& redBranch, const T& key, Queue*& q, bool& finish, Tree* last)
{
	bool out = false;
	Tree* outnode = 0x0;
	redBranch->color = black;
	if (key >= *redBranch->key) do_R_downRed(redBranch, out, outnode, last);
	else do_L_downRed(redBranch, out, outnode, last);
	if (key == *outnode->key) {
		delete outnode->key; delete outnode;
		finish = true;
	}
	else { redBranch = outnode; }
	return true;
}
template<typename T>
bool LLRB<T>::do_L_downRed(Tree*& p, bool& out, Tree*& outnode, Tree* last)
{
	if (!p)return false;
	do_L_downRed(p->l, out, outnode, p);
	do_L_downRed(p->r, out, outnode, p);
	if (!p->l) { p->color = red; }
	if (!out && !p->l) {
		outnode = p;  out = true; last->l = last->r; last->r = 0x0;
		int* temp = last->key; last->key = last->l->key; last->l->key = temp;
		last->l->color = red;
		return true;
	}
	if (p->r) { toBalance(last->l); toBalance(last->r); }
	return true;
}
template<typename T>
bool LLRB<T>::do_R_downRed(Tree*& p, bool& out, Tree*& outnode, Tree* last)
{
	if (!p)return false;
	do_R_downRed(p->r, out, outnode, p);
	do_R_downRed(p->l, out, outnode, p);
	if (!p->l) { p->color = red; }
	if (!out && !p->r) {
		outnode = p; last->r = 0x0; last->l->color = red; out = true; return true;
	}
	if (p->r && last) { toBalance(last->l); toBalance(last->r); }//因为要用此操作树坍塌,增加根情况
	return true;
}

//检查组件
template<typename T>
bool LLRB<T>::getBlance(Tree* p, int deep, int truedeep)
{
	if (!p) { if (deep != truedeep)throw"deep_err"; return true; }
	if (p->color)++deep;
	getBlance(p->l, deep, truedeep);
	getBlance(p->r, deep, truedeep);
	return true;
}
template<typename T>
bool LLRB<T>::showTree()
{
	do_show(root);
	return true;
}
template<typename T>
bool LLRB<T>::do_show(Tree* p)
{
	if (!p)return 1;
	do_show(p->l);
	std::cout << *p->key << "	";
	do_show(p->r);
	return 1;
}

//可选功能,调用showQ类
template<typename T>
void LLRB<T>::Qs(Tree* p)
{
	if (!p)p = root;
	showQue s;
	int deep = 0;
	Que(p, s, deep);
	s.show();
}
template<typename T>
void LLRB<T>::Que(Tree* p, showQue& s, int deep)
{
	if (!p)return;
	std::ostringstream ss;
	ss << *p->key;
	std::string str = ss.str();
	if (p->color) { str += "-X "; }
	else { str += "-O "; }
	s.in(deep, str);
	++deep;
	Que(p->l, s, deep);
	Que(p->r, s, deep);
}
template<typename T>
int LLRB<T>::CheckBalance()
{
	showQue showQ;
	int truedeep = 0; Tree* getdeep = root;
	while (getdeep) { if (getdeep->color)++truedeep; getdeep = getdeep->l; }
	int deep = 0;
	getBlance(root, deep, truedeep);
	return truedeep;
}

//重载组件
template<typename T>
LLRB<T>& LLRB<T>::operator+(const T& key) { in(key); }
template<typename T>
LLRB<T>& LLRB<T>::operator-(const T& key) { out(key); }

#endif // !myLLRB

//测试器
/*
#include<algorithm>
int main() {
	int i[256];
	for (int n = 0; n < 256; ++n)i[n] = (n + 1);
	std::random_shuffle(&i[0], &i[255]);
	for (int n = 0; n < 256; ++n)std::cout << i[n] << "	";
	std::cout << '\n';
	LLRB<int> rbTree(0);
	for (int n = 0; n < 256; ++n)
		rbTree.in(i[n]);
	rbTree.CheckBalance();
	rbTree.showTree();
	std::cout << '\n';
	rbTree.Qs();
	int n = 0;
	for (n = 0; n < 256; ++n) {  printf("%d-%d ", n, i[n]); };
	for (int n = 0; n < 256; ++n) { rbTree.out(i[n]); }
	system("pause");
	return 0;

}
*/
