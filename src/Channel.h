//
// Created by Grant Horner on 8/20/22.
//

#ifndef PLAINSCAPE_CHANNEL_H
#define PLAINSCAPE_CHANNEL_H

#include <queue>
#include <mutex>

template<typename T>
class Channel {
public:
	Channel() noexcept = default;
	void put(T x) noexcept;
	void take() noexcept;
	bool empty() noexcept;
private:
	std::queue<T> m_queue;
	std::mutex m_mutex;
};

template<typename T>
void Channel<T>::put(T x) noexcept {
	m_mutex.lock();
	m_queue.push(x);
	m_mutex.unlock();
}

template<typename T>
void Channel<T>::take() noexcept {
	while (empty()) {}
	m_mutex.lock();
	auto x = m_queue.front();
	m_queue.pop();
	m_mutex.unlock();
	return x;
}

template<typename T>
bool Channel<T>::empty() noexcept {
	return m_queue.empty();
}

#endif //PLAINSCAPE_CHANNEL_H
