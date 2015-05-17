/**
 * @file       TFactory.h
 * @ingroup    Base
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Yann Andrey <yann.andrey@hotmail.fr>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/
#pragma once

#include <memory>
#include <map>
#include <vector>
#include <algorithm>

namespace Base
{
	template<typename T>
	typename T::first_type select1st_factory(const T& aValue)
	{
		return aValue.first;
	}

	template <typename KeyT, typename BaseT, typename CompareT = std::less<KeyT> >
	class TFactory
	{
	public:
		typedef std::unique_ptr<BaseT> UniquePtr_t;
		typedef std::shared_ptr<BaseT> SharedPtr_t;

	private:
		class FactoryBase
		{
		public:
			typedef std::unique_ptr<FactoryBase> BaseUniquePtr_t;

		public:
			FactoryBase(void)
			{
			}
			virtual ~FactoryBase(void)
			{
			}
			virtual UniquePtr_t makeUniqueNewInstance(void) const = 0;
			virtual SharedPtr_t makeSharedNewInstance(void) const = 0;
		};
		template <typename TypeT>
		class TFactoryInternal : public FactoryBase
		{
		public:
			TFactoryInternal(void)
				: FactoryBase()
			{
			}
			virtual UniquePtr_t makeUniqueNewInstance(void) const
			{
				return std::make_unique<TypeT>();
			}
			virtual SharedPtr_t makeSharedNewInstance(void) const
			{
				return std::make_shared<TypeT>();
			}
		};

	public:
		TFactory(void)
			: mFactoryMap()
		{
		}
		template <typename TypeT>
		bool registerPlugin(const KeyT& aName)
		{
			mFactoryMap.insert(std::make_pair(aName, std::make_unique<TFactoryInternal<TypeT> >()));
			return true;
		}
		UniquePtr_t makeUniqueNewInstance(const KeyT& aName) const
		{
			FactoryStringMap_t::const_iterator itFactory = mFactoryMap.find(aName);
			if (itFactory == mFactoryMap.end())
				return nullptr;
			return itFactory->second->makeUniqueNewInstance();
		}
		SharedPtr_t makeSharedNewInstance(const KeyT& aName) const
		{
			FactoryStringMap_t::const_iterator itFactory = mFactoryMap.find(aName);
			if (itFactory == mFactoryMap.end())
				return nullptr;
			return itFactory->second->makeSharedNewInstance();
		}
		void getKeys(std::vector<KeyT>& aKeysResult) const
		{
			aKeysResult.resize(mFactoryMap.size());
			std::transform(mFactoryMap.begin(), mFactoryMap.end(), aKeysResult.begin(), select1st_factory<FactoryStringMap_t::value_type>);
		}

	private:
		TFactory(const TFactory& aCopy);
		TFactory& operator=(const TFactory& aCopy);
		typedef std::map<KeyT, typename FactoryBase::BaseUniquePtr_t, CompareT> FactoryStringMap_t;
		FactoryStringMap_t mFactoryMap;
	};
};
