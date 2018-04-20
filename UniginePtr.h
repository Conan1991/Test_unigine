/* Copyright (C) 2005-2014, Unigine Corp. All rights reserved.
 *
 * File:    UniginePtr.h
 * Desc:    Unigine API
 * Version: 1.06
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#ifndef __UNIGINE_PTR_H__
#define __UNIGINE_PTR_H__

#include "UnigineMemory.h"

/**
 * Unigine namespace.
 */
namespace Unigine {
	
	/**
	 * Smart pointer template.
	 */
	template <class Type> class Ptr {
			
		public:
			
			/**
			 * Default constructor that produces a NULL pointer.
			 */
			Ptr() : pointer(0), counter(0) { }
			/**
			 * Copy constructor.
			 *
			 * @param ptr Pointer of the given type.
			 */
			Ptr(const Ptr<Type> &ptr) : pointer(ptr.pointer), counter(ptr.counter) { grab(); }
			/**
			 * Constructor.
			 *
			 * @param ptr Pointer of the given type.
			 */
			explicit Ptr(Type *ptr) : pointer(ptr), counter(0) { grab(); }
			/**
			 * Virtual destructor.
			 */
			virtual ~Ptr() { release(); }
			
			/**
			 * Assignment operator.
			 *
			 * @param ptr Pointer of the given type.
			 * @return Pointer.
			 */
			Ptr<Type> &operator=(const Ptr<Type> &ptr) {
				if(this == &ptr) return *this;
				release();
				pointer = ptr.pointer;
				counter = ptr.counter;
				grab();
				return *this;
			}
			
			/**
			 * Access to the pointer.
			 *
			 * @return Pointer.
			 */
			UNIGINE_INLINE Type *operator->() const {
				UNIGINE_ASSERT(pointer && "Unigine::Ptr::operator->(): pointer is NULL");
				UNIGINE_ASSERT(((*counter) & DESTROY_FLAG) == 0 && "Unigine::Ptr::operator->(): pointer is destroyed");
				return pointer;
			}
			/**
			 * Return the internal address.
			 *
			 * @return Pointer.
			 */
			UNIGINE_INLINE Type *get() const {
				if(counter && ((*counter) & DESTROY_FLAG)) return NULL;
				return pointer;
			}
			
			/**
			 * Return the reference counter.
			 *
			 * @return Reference counter.
			 */
			UNIGINE_INLINE int getCounter() const {
				if(counter) return (*counter) & COUNTER_MASK;
				return 0;
			}
			
			/**
			 * Clear the pointer.
			 */
			UNIGINE_INLINE void clear() {
				release();
				pointer = 0;
				counter = 0;
			}
			/**
			 * Destroy the pointer.
			 */
			UNIGINE_INLINE void destroy() {
				release();
				if(counter && ((*counter) & DESTROY_FLAG) == 0) {
					Type::release_ptr(pointer);
					*counter |= DESTROY_FLAG;
				}
				pointer = 0;
				counter = 0;
			}
			
		private:
			
			/// @cond
			
			enum {
				DESTROY_FLAG = (int)(1 << (sizeof(int) * 8 - 1)),
				COUNTER_MASK = ~(int)DESTROY_FLAG,
			};
			
			void grab() {
				if(pointer) {
					if(counter) {
						int destroyed = (*counter) & DESTROY_FLAG;
						*counter = ((*counter) & COUNTER_MASK) + 1;
						if(destroyed) *counter |= DESTROY_FLAG;
					} else {
						counter = (int*)Memory::allocate(sizeof(int));
						*counter = 1;
					}
				}
			}
			
			void release() {
				if(counter) {
					int destroyed = (*counter) & DESTROY_FLAG;
					*counter = ((*counter) & COUNTER_MASK) - 1;
					if(destroyed) *counter |= DESTROY_FLAG;
					if(((*counter) & COUNTER_MASK) == 0) {
						if(destroyed == 0) Type::release_ptr(pointer);
						Memory::deallocate(counter);
						pointer = 0;
						counter = 0;
					}
				}
			}
			
			Type *pointer;
			int *counter;
			
			/// @endcond
	};
	
} /* namespace Unigine */

#endif /* __UNIGINE_PTR_H__ */
