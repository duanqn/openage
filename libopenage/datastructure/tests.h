// Copyright 2014-2020 the openage authors. See copying.md for legal info.

#pragma once

#include <functional>
#include <stddef.h>
#include <mutex>

namespace openage::datastructure::tests {

/**
 * simplest priority queue element that supports reordering.
 */
struct heap_elem {
	int data;

	bool operator <(const heap_elem &other) const {
		return this->data < other.data;
	}

	bool operator ==(const heap_elem &other) const {
		return this->data == other.data;
	}
};

/**
 * A simple class that can be move-constructed but not copy-constructed
 */
class MoveOnly{
	private:
	int m_data;
	public:
	MoveOnly(int data): m_data(data) {}
	MoveOnly(const MoveOnly &) = delete;
	MoveOnly(MoveOnly&& other): m_data(other.get()) {}
	~MoveOnly() {}

	int get() const {
		return m_data;
	}
};

/**
 * A simple class that can be copy-constructed but not move-constructed
 */
class CopyOnly{
	private:
	int m_data;
	public:
	CopyOnly(int data): m_data(data) {}
	CopyOnly(const CopyOnly & other): m_data(other.get()) {}
	CopyOnly(CopyOnly&&) = delete;
	~CopyOnly() {}

	int get() const {
		return m_data;
	}
};

/**
 * A simple class that can be both copy-constructed and move-constructed
 */
class CopyMove{
	private:
	int m_data;

	static std::mutex s_mutex;
	static int s_accumulatedConstructionCounter;

	void atomicInc() {
		std::scoped_lock lock(s_mutex);
		s_accumulatedConstructionCounter++;
	}

	public:
	CopyMove(int data): m_data(data) {
		atomicInc();
	}

	CopyMove(const CopyMove & other): m_data(other.get()) {
		atomicInc();
	}

	// Move constructor does not increase global counter
	CopyMove(CopyMove&& other): m_data(other.get()) {}

	~CopyMove() {}

	int get() const {
		return m_data;
	}

	static int getAccumulatedConstructionCounter() {
		return s_accumulatedConstructionCounter;
	}

	static void resetAccumulatedConstructionCounter() {
		s_accumulatedConstructionCounter = 0;
	}
};

} // namespace openage::datastructure::tests

namespace std {

/**
 * hash function for the simple heap_elem
 */
template<>
struct hash<openage::datastructure::tests::heap_elem> {
	size_t operator ()(const openage::datastructure::tests::heap_elem &elem) const {
		return elem.data;
	}
};
} // namespace std
