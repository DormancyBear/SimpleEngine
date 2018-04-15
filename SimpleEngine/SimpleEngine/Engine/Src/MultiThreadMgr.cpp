#include "MultiThreadMgr.h"

void MultiThreadMgr::QueueOnMainThread(std::function<void(void)> action)
{
	// lock_guard 实例创建时会尝试持有 mutex( 本质还是调用 mutex::lock )
	// lock_guard 实例销毁时自动释放 mutex( mutex::unlock )
	// 此处 lg 为栈对象，会在函数结束时自动销毁
	// lock_guard 的实现思想: RAII => 对象的构造函数完成资源的获取, 析构函数完成资源的释放
	// C++ Runtime 保证了所有栈对象会在生命周期结束时调用其析构函数( 即使函数抛出异常 )
	// RAII 保证了函数即使抛出异常, 资源也能正常释放 =>
	// 如果把资源释放单独做一个语句放在函数最后, 一旦抛出异常, 走不到最后一句, 资源就被锁死了
	// 尽量不要写 try-finally
	std::lock_guard<std::mutex> lg(getInstance().actionsMutex);

	getInstance()._actions.push_back(action);
}

void MultiThreadMgr::RunAsyncInDetach(std::function<void(void)> action)
{
	// std::thread 作为一个类, 本来的写法应该是 std::thread t(action)
	// 但这样写编译器会把它当成一条函数声明( 返回类型为 std::thread 的函数 )
	// 所以改用大括号
	// 线程函数之于子线程 = main() 函数之于主线程
	std::thread t{ action };
	t.detach();
}

void MultiThreadMgr::Update()
{
	std::lock_guard<std::mutex> lg(actionsMutex);

	for (size_t i = 0; i < _actions.size(); i++)
	{
		_actions[i]();
	}
	_actions.clear();
}