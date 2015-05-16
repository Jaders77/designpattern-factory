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
#include <string>
#include <map>

namespace Base
{
	template <typename TypeBaseT>
	class TFactory
	{
	public:
		typedef std::unique_ptr<TypeBaseT> UniquePtr_t;
		typedef std::shared_ptr<TypeBaseT> SharedPtr_t;

	private:
		class FactoryBase
		{
		public:
			typedef std::unique_ptr<FactoryBase> UniquePtr_t;

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
		bool registerPlugin(const std::string& aName)
		{
			mFactoryMap.insert(std::make_pair(aName, std::make_unique<TFactoryInternal<TypeT> >()));
			return true;
		}
		UniquePtr_t makeUniqueNewInstance(const std::string& aName) const
		{
			FactoryStringMap_t::const_iterator itFactory = mFactoryMap.find(aName);
			if (itFactory == mFactoryMap.end())
				return nullptr;
			return itFactory->second->makeUniqueNewInstance();
		}
		SharedPtr_t makeSharedNewInstance(const std::string& aName) const
		{
			FactoryStringMap_t::const_iterator itFactory = mFactoryMap.find(aName);
			if (itFactory == mFactoryMap.end())
				return nullptr;
			return itFactory->second->makeSharedNewInstance();
		}

	private:
		TFactory(const TFactory& aCopy);
		TFactory& operator=(const TFactory& aCopy);
		typedef std::map<std::string, typename FactoryBase::UniquePtr_t> FactoryStringMap_t;
		FactoryStringMap_t mFactoryMap;
	};
};
