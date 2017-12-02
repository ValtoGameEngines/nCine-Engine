#ifndef CLASS_NCINE_HASHMAPITERATOR
#define CLASS_NCINE_HASHMAPITERATOR

#include "HashMap.h"
#include "List.h"
#include "iterator_traits.h"

namespace ncine {

/// Base helper structure for type traits used in the hashmap iterator
template <class K, class T, class HashFunc, bool IsConst>
struct HelperTraits { };

/// Helper structure providing type traits used in the non constant hashmap iterator
template <class K, class T, class HashFunc>
struct HelperTraits<K, T, HashFunc, false>
{
	using HashMapPtr = HashMap<K, T, HashFunc> *;
	using BucketPtr = typename HashMap<K, T, HashFunc>::HashBucket *;
	using ListIteratorType = typename List<HashMapNode<K, T> >::Iterator;
	using NodeReference = HashMapNode<K, T> &;
};

/// Helper structure providing type traits used in the constant hashmap iterator
template <class K, class T, class HashFunc>
struct HelperTraits<K, T, HashFunc, true>
{
	using HashMapPtr = const HashMap<K, T, HashFunc> *;
	using BucketPtr = const typename HashMap<K, T, HashFunc>::HashBucket *;
	using ListIteratorType = typename List<HashMapNode<K, T> >::ConstIterator;
	using NodeReference = const HashMapNode<K, T> &;
};

/// A hashmap iterator
template <class K, class T, class HashFunc, bool IsConst>
class HashMapIterator
{
  public:
	/// Reference type which respects iterator constness
	using Reference = typename IteratorTraits<HashMapIterator>::Reference;

	HashMapIterator(typename HelperTraits<K, T, HashFunc, IsConst>::HashMapPtr hashMap, unsigned int bucketIndex,
	                typename HelperTraits<K, T, HashFunc, IsConst>::ListIteratorType listIterator, bool atFirstNode)
		: hashMap_(hashMap), bucketIndex_(bucketIndex), listIterator_(listIterator), atFirstNode_(atFirstNode) { }

	/// Copy constructor to implicitly convert a non constant iterator to a constant one
	HashMapIterator(const HashMapIterator<K, T, HashFunc, false> &it)
		: hashMap_(it.hashMap_), bucketIndex_(it.bucketIndex_), listIterator_(it.listIterator_), atFirstNode_(it.atFirstNode_) { }

	/// Deferencing operator
	Reference operator*() const;

	/// Iterates to the next element (prefix)
	HashMapIterator &operator++();
	// Iterates to the next element (postfix)
	HashMapIterator operator++(int);

	/// Iterates to the previous element (prefix)
	HashMapIterator &operator--();
	/// Iterates to the previous element (postfix)
	HashMapIterator operator--(int);

	/// Equality operator
	friend inline bool operator==(const HashMapIterator &lhs, const HashMapIterator &rhs)
	{
		return (lhs.hashMap_ == rhs.hashMap_ && lhs.bucketIndex_ == rhs.bucketIndex_ &&
		        lhs.listIterator_ == rhs.listIterator_ && lhs.atFirstNode_ == rhs.atFirstNode_);
	}

	/// Inequality operator
	friend inline bool operator!=(const HashMapIterator &lhs, const HashMapIterator &rhs)
	{
		return (lhs.hashMap_ != rhs.hashMap_ || lhs.bucketIndex_ != rhs.bucketIndex_ ||
		        lhs.listIterator_ != rhs.listIterator_ || lhs.atFirstNode_ != rhs.atFirstNode_);
	}

	/// Returns the hashmap node currently pointed by the iterator
	typename HelperTraits<K, T, HashFunc, IsConst>::NodeReference node() const;
	/// Returns the value associated to the currently pointed node
	const T &value() const;
	/// Returns the key associated to the currently pointed node
	const K &key() const;
	/// Returns the hash associated to the currently pointed node
	hash_t hash() const;

  private:
	typename HelperTraits<K, T, HashFunc, IsConst>::HashMapPtr hashMap_;
	unsigned int bucketIndex_;
	typename HelperTraits<K, T, HashFunc, IsConst>::ListIteratorType listIterator_;
	bool atFirstNode_;

	/// Makes the iterator point to the next element in the hashmap
	void next();
	/// Makes the iterator point to the previous element in the hashmap
	void previous();

	/// For non constant to constant iterator implicit conversion
	friend class HashMapIterator<K, T, HashFunc, true>;
};

/// Iterator traits structure specialization for `HashMapIterator` class
template <class K, class T, class HashFunc>
struct IteratorTraits<HashMapIterator<K, T, HashFunc, false> >
{
	/// Type of the values deferenced by the iterator
	using ValueType = T;
	/// Pointer to the type of the values deferenced by the iterator
	using Pointer = T *;
	/// Reference to the type of the values deferenced by the iterator
	using Reference = T &;
	/// Type trait for iterator category
	static inline BidirectionalIteratorTag IteratorCategory() { return BidirectionalIteratorTag(); }
};

/// Iterator traits structure specialization for constant `HashMapIterator` class
template <class K, class T, class HashFunc>
struct IteratorTraits<HashMapIterator<K, T, HashFunc, true> >
{
	/// Type of the values deferenced by the iterator (never const)
	using ValueType = T;
	/// Pointer to the type of the values deferenced by the iterator
	using Pointer = const T *;
	/// Reference to the type of the values deferenced by the iterator
	using Reference = const T &;
	/// Type trait for iterator category
	static inline BidirectionalIteratorTag IteratorCategory() { return BidirectionalIteratorTag(); }
};

template <class K, class T, class HashFunc, bool IsConst>
inline typename HashMapIterator<K, T, HashFunc, IsConst>::Reference HashMapIterator<K, T, HashFunc, IsConst>::operator*() const
{
	return node().value;
}

template <class K, class T, class HashFunc, bool IsConst>
HashMapIterator<K, T, HashFunc, IsConst> &HashMapIterator<K, T, HashFunc, IsConst>::operator++()
{
	next();
	return *this;
}

template <class K, class T, class HashFunc, bool IsConst>
HashMapIterator<K, T, HashFunc, IsConst> HashMapIterator<K, T, HashFunc, IsConst>::operator++(int)
{
	// Create an unmodified copy to return
	HashMapIterator<K, T, HashFunc, IsConst> iterator = *this;
	next();
	return iterator;
}

template <class K, class T, class HashFunc, bool IsConst>
HashMapIterator<K, T, HashFunc, IsConst> &HashMapIterator<K, T, HashFunc, IsConst>::operator--()
{
	previous();
	return *this;
}

template <class K, class T, class HashFunc, bool IsConst>
HashMapIterator<K, T, HashFunc, IsConst> HashMapIterator<K, T, HashFunc, IsConst>::operator--(int)
{
	// Create an unmodified copy to return
	HashMapIterator<K, T, HashFunc, IsConst> iterator = *this;
	previous();
	return iterator;
}

template <class K, class T, class HashFunc, bool IsConst>
typename HelperTraits<K, T, HashFunc, IsConst>::NodeReference HashMapIterator<K, T, HashFunc, IsConst>::node() const
{
	if (atFirstNode_)
		return hashMap_->buckets_[bucketIndex_].firstNode_;
	else
		return *listIterator_;
}

template <class K, class T, class HashFunc, bool IsConst>
inline const T &HashMapIterator<K, T, HashFunc, IsConst>::value() const
{
	return node().value;
}

template <class K, class T, class HashFunc, bool IsConst>
inline const K &HashMapIterator<K, T, HashFunc, IsConst>::key() const
{
	return node().key;
}

template <class K, class T, class HashFunc, bool IsConst>
inline hash_t HashMapIterator<K, T, HashFunc, IsConst>::hash() const
{
	return node().hash;
}

template <class K, class T, class HashFunc, bool IsConst>
void HashMapIterator<K, T, HashFunc, IsConst>::next()
{
	typename HelperTraits<K, T, HashFunc, IsConst>::BucketPtr bucket = &(hashMap_->buckets_[bucketIndex_]);

	if (atFirstNode_)
	{
		atFirstNode_ = false;
		listIterator_ = bucket->collisionList_.begin();
	}
	else
		listIterator_++;

	// The list iterator condition also applies when it points to the beginning of an empty list
	if (listIterator_ == bucket->collisionList_.end() && bucketIndex_ < hashMap_->buckets_.size() - 1)
	{
		do
		{
			bucket = &(hashMap_->buckets_[++bucketIndex_]);
		}
		while (bucketIndex_ < hashMap_->buckets_.size() - 1 && bucket->size() == 0);

		if (bucket->size() > 0)
			atFirstNode_ = true;
	}
}

template <class K, class T, class HashFunc, bool IsConst>
void HashMapIterator<K, T, HashFunc, IsConst>::previous()
{
	typename HelperTraits<K, T, HashFunc, IsConst>::BucketPtr bucket = &(hashMap_->buckets_[bucketIndex_]);

	if (atFirstNode_ && bucketIndex_ > 0)
	{
		do
		{
			bucket = &(hashMap_->buckets_[--bucketIndex_]);
		}
		while (bucketIndex_ > 0 && bucket->size() == 0);

		if (bucket->size() > 0)
		{
			atFirstNode_ = false;
			listIterator_ = bucket->collisionList_.end();
		}
	}
	else
		listIterator_--;

	if (listIterator_ == bucket->collisionList_.begin())
		atFirstNode_ = true;
}

}

#endif
