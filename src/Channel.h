//
// Created by Grant Horner on 8/20/22.
//

#ifndef PLAINSCAPE_CHANNEL_H
#define PLAINSCAPE_CHANNEL_H

#include <queue>
#include <mutex>
#include <optional>

template<typename T>
class Channel {
public:
	explicit Channel(bool& should_exit) : m_should_exit(should_exit) {};
	void put(T x) noexcept;
	std::optional<T> take() noexcept;
	bool empty() noexcept;
private:
	std::queue<T> m_queue;
	std::mutex m_mutex;
	bool& m_should_exit;
};

template<typename T>
void Channel<T>::put(T x) noexcept {
	while (!empty()) {
		if (m_should_exit)
			return;
	}
	m_mutex.lock();
	m_queue.push(x);
	m_mutex.unlock();
}

template<typename T>
std::optional<T> Channel<T>::take() noexcept {
	while (empty()) {
		if (m_should_exit)
			return std::nullopt;
	}
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
