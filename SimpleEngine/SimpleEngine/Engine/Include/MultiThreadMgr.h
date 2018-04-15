#pragma once

#ifndef __MULTITHREADMGR_H
#define __MULTITHREADMGR_H

#include <vector>
#include <functional>
#include <mutex>

class MultiThreadMgr
{
public:
	static MultiThreadMgr& getInstance()
	{
		// 局部静态变量
		// lifetime =>
		// begins the first time the program flow encounters the declaration
		// ends at program termination( main 函数结束后调用析构函数 )
		// 保证这个实例能够随着程序退出 correctly destroyed
		// 只有第一次 declare 会分配内存并初始化( 调用构造函数 )，之后直接返回已初始化的静态变量
		static MultiThreadMgr instance;
		return instance;
	}

	// 显式指示编译器不生成函数的默认版本
	// deleting the methods we don't want
	MultiThreadMgr(MultiThreadMgr const&) = delete;
	void operator=(MultiThreadMgr const&) = delete;

private:
	// 隐藏以下函数: 必须通过 GetInstance() 创建实例
	MultiThreadMgr() {}

public:
	// Queues an action on the main thread
	static void QueueOnMainThread(std::function<void(void)> action);

	// Runs an action on another thread
	static void RunAsyncInDetach(std::function<void(void)> action);

	// Update is called once per frame on the main thread
	void Update();

private:
	// 原生的函数指针太过简陋, std::function 在其基础上加了一层封装
	std::vector<std::function<void(void)>> _actions;
	std::vector<std::function<void(void)>> runActions;

	std::mutex actionsMutex;
};

#endif // !__MULTITHREADMGR_H