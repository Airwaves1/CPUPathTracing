#pragma once

#include <atomic>
#include <thread>

// 自旋锁是一种简单的锁机制，它不会使线程睡眠，而是让线程在锁被占用时不断轮询，直到锁被释放。

// test_and_set：这是一个原子操作，它做两件事：返回 m_Flag 之前的值。将 m_Flag 设置为 true。

// std::memory_order_acquire：这是一个内存序，确保在获取锁之后的所有读写操作不会被重排序到获取锁之前。

// m_Flag.test_and_set(std::memory_order_acquire) 是一个原子操作，用于测试并设置 m_Flag 的值。它返回 m_Flag 之前的值，并将 m_Flag 设置为 true。
// 这是一个原子操作，意味着它是不可分割的，确保在多线程环境中不会出现竞态条件。

class SpinLock
{
public:
    // 获取锁
    void Acquire()
    {
        // 试图获取锁。如果锁已被占用（即 m_Flag 已设置），则不断轮询（忙等待）
        while (m_Flag.test_and_set(std::memory_order_acquire))
        {
            // 让出当前线程的执行权限，让操作系统调度其他线程
            std::this_thread::yield();
        }
    }

    // 释放锁
    void Release()
    {
        // 清除标志位，表示锁已被释放
        m_Flag.clear(std::memory_order_release);
    }

private:
    // 原子标志，表示锁的状态。atomic_flag 是一个简单的原子布尔类型，默认值是 false
    std::atomic_flag m_Flag{};
};

// Guard 类，用于自动管理锁的获取和释放
class Guard
{
public:
    Guard(SpinLock &lock) : m_Lock(lock)
    {
        m_Lock.Acquire();
    }

    ~Guard()
    {
        m_Lock.Release();
    }

private:
    SpinLock &m_Lock;
};