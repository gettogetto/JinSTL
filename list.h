#ifndef JIN_LIST_H_
#define JIN_LIST_H_
#include"iterator.h"
#include"algobase.h"
#include <stddef.h>
#include "iterator.h"
#include "alloc.h"
#include "type_traits.h"
#include "uninitialized.h"
#include "construct.h"
namespace jinstl{
template<class T>
struct list_node{
	list_node(T value = 0, list_node<T>* p = nullptr, list_node<T>* n = nullptr)
			:val(value), pre(p), next(n) {}
	T val;
	list_node<T> *pre,*next;
	
};

template <class T, class Ref, class Ptr>
	struct list_iterator : public iterator<bidirectional_iterator_tag, T> {
		typedef bidirectional_iterator_tag 	iterator_category;
		typedef T				value_type;
		typedef ptrdiff_t 			difference_type;
		typedef Ptr				pointer;
		typedef Ref				reference;
		

		typedef list_iterator<T, T&, T*>	iterator;
		typedef list_iterator<T, const T&, const T*>	const_iterator;
		typedef list_iterator<T, Ref, Ptr>	self;
		typedef size_t size_type;
		
		
		list_node<T>*	m_node;//one list one empty node	

		list_iterator() {}
		list_iterator(list_node<T>* x) :m_node(x) {}
		list_iterator(const iterator& rhs) :m_node(rhs.m_node) {}

		bool operator==(const self& rhs) const { return m_node == rhs.m_node; }
		bool operator!=(const self& rhs) const { return m_node != rhs.m_node; }
		reference operator*() const { return m_node->val; }
		pointer operator->() const { return &(m_node->val); }
		self& operator++() {
			m_node = m_node->next;
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}
		self& operator--() {
			m_node = m_node->pre;
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}
	};


//cycle list
template <class T, class Alloc = alloc>
	class list {
	public:
		typedef T		value_type;
		typedef Alloc	allocator_type;
		typedef value_type*	pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&	reference;
		typedef const value_type&	const_reference;
		typedef size_t	size_type;
		typedef ptrdiff_t	difference_type;

		typedef list_iterator<T, T&, T*>	iterator;
		typedef list_iterator<T, const T&, const T*>	const_iterator;
		//typedef reverse_iterator<const_iterator>	const_reverse_iterator;
		//typedef reverse_iterator<iterator>	reverse_iterator;

	

	public:
		typedef simple_alloc<list_node<T>, Alloc>	data_allocator;	
	//	allocator_type get_allocator() { return allocator_type(); }

	private:
		list_node<T>* m_node;	

	public:
		list() { __empty_initialize(); }
		explicit list(size_type n){__fill_initialize(n,T());}
		list(size_type n, const T& val){__fill_initialize(n,val);}
		template <class InputIterator>
		list(InputIterator first, InputIterator last){
			__range_initialize(first,last);
		}

		list(const list& rhs){
			__range_initialize(rhs.begin(),rhs.end());
		}
		list(list&& rhs){
			__range_initialize(rhs.begin(),rhs.end());
		}

		list& operator=(const list& rhs){
			if(this == &rhs) return *this;
			__empty_initialize();
			m_node = rhs.m_node;
			return *this;
		}
		list& operator=(list&& rhs){
			swap(m_node,rhs.m_node);
			return *this;	
		}

		~list(){
			//destroy(begin(),end());
			//data_allocator::deallocate(begin().m_node,size());
			clear();
			__put_node(m_node);
			
		}

	public:
		iterator begin() { return iterator(m_node->next); }
		const_iterator begin() const { return const_iterator(m_node->next); }
		iterator end() { return iterator(m_node); }
		const_iterator end() const { return const_iterator(m_node); }
	//	reverse_iterator rbegin() { return reverse_iterator(end()); }
	//	const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
	//	reverse_iterator rend() { return reverse_iterator(begin()); }
	//	const_reverse_iterator rend() const { return reverse_iterator(begin()); }

		bool empty() const { return m_node->next == m_node; }
		size_type size() const { return static_cast<size_type>(distance(begin(), end())); }
		size_type max_size() const { return static_cast<size_type>(-1); }

		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(--end()); }
		const_reference back() const { return *(--end()); }

		//void assign(size_type n, const T& value) { __fill_assign(n, value); }
		//void assign(size_type n) { __fill_assign(n, T()); }
		//template <class InputIterator>
		//void assign(InputIterator first, InputIterator last);
		iterator insert(iterator pos, const T& val){
			list_node<T>* newnode = __create_node(val);

			list_node<T>* posnode = pos.m_node;
			list_node<T>* prenode = posnode->pre;
			newnode->next = posnode;
			posnode->pre = newnode;
			prenode->next = newnode;
			newnode->pre = prenode;
			
			return iterator(newnode);
			/*list_node<T>* tmp = __create_node(val);
			tmp->next = pos.m_node;
			tmp->pre = pos.m_node->pre;
			pos.m_node->pre->next = tmp;
			pos.m_node->pre = tmp;
			return tmp;*/
		}
		iterator insert(iterator position) { return insert(position, T()); }
		
		void insert(iterator pos, size_type n, const T& x){
			while(n--){
				insert(pos,x);
			}	

		}
		template <class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last){
			while(first!=last){
				insert(pos,T(*first));
				++first;
			}

		}
		iterator erase(iterator position){
			/*iterator res =iterator(pos.m_node->next);
			pos.m_node->pre->next = pos.m_node->next;
			pos.m_node->next->pre = pos.m_node->pre;
			__destroy_node(pos.m_node);
			return res;*/
			list_node<T>* prev_node = position.m_node->pre;
			list_node<T>* next_node = position.m_node->next;
			list_node<T>* this_node = position.m_node;
			prev_node->next = next_node;
			next_node->pre = prev_node;
			__destroy_node(this_node);
			return iterator(next_node);
		}
		iterator erase(iterator first, iterator last){
			/*list_node<T>* firstpre = first.m_node->pre;
			list_node<T>* lastpre = last.m_node->pre;
			firstpre->next=last.m_node;
			last.m_node->pre = firstpre;
			
			while(true ){
				if(first == lastpre){
					__destroy_node(first.m_node);
					break;
				}
				__destroy_node(first.m_node);
				first++;
			}	*/
			while(first!=last)  erase(first++);
			return last;

		}
		void clear(){
			list_node<T>* cur = begin().m_node;
			while(cur!=end().m_node){
				list_node<T>* tmp = cur;
				cur = cur->next;
				destroy(tmp);
			}
			m_node->next = m_node;
			m_node->pre=m_node;
		}
		void push_front(const T& x) { insert(begin(), x); }
		void push_back(const T& x) { insert(end(), x); }
		void pop_front() { erase(begin()); }
		void pop_back() { iterator tmp = end(); erase(--tmp); }
		void resize(size_type new_size, const T& x){
			if(new_size>size()){
				size_type add = new_size-size();
				while(add--){
					insert(end(),x);
				}
			}else{
				iterator i=begin();
				while(new_size--) i++; 
				erase(i,end());
			}
			
		}
		void resize(size_type new_size) { resize(new_size, T()); }
		void swap(list& rhs) { jinstl::swap(m_node, rhs.m_node); }

		
		
		void splice(iterator pos, list& alist){
			if(!alist.empty()) __transfer(pos,alist.begin(),alist.end());			
			
		}
		void splice(iterator pos, list&, iterator i){
			if(pos.m_node!=i.m_node&&pos.m_node!=i.m_node->next) 
				__transfer(pos,iterator(i.m_node),iterator(i.m_node->next));
		}
//move [first,last) to [pos-(last-first),pos ),and [first last) will be empty
		void splice(iterator pos, list& x, iterator first, iterator last){
			if(first!=last) __transfer(pos,first,last);
		}
		void remove(const T& val){
			iterator it;
			for(it=begin();it!=end();it++){
				if(*it==val){
					if(it == begin()){//if erase the first iterator
						it = erase(it);
						m_node = it.m_node;
					}else{
						it = erase(it);	
					}
					break;
				}
			}
		}
		template <class Predicate>
		void remove_if(Predicate pred){
			for(iterator it=begin();it!=end();it++){
				if(pred(*it)){
					if(it==begin()){
						it=erase(it);
						m_node=it.m_node;
					}else{
						it=erase(it);
					}
					break;
				}
			}
		}
		void unique(){
			iterator pre = begin();
			iterator cur = ++begin();
			while(cur!=end()){
				if(*cur==*pre) cur = erase(cur);	
				else{
					pre++;
					cur++;
				}
			}
			
		}
		template<class Predicate>
		void unique(Predicate predicate){
			iterator pre = begin();
			iterator cur = ++begin();
			while(cur!=end()){
				if(predicate(*pre,*cur)) cur = erase(cur);	
				else{
					pre++;
					cur++;
				}
			}
		}
			
		void merge(list& x){//after merge x will be empty!
			iterator first1=begin();
			iterator last1=end();
			iterator first2=x.begin();
			iterator last2=x.end();
			while(first1!=last1&&first2!=last2){
				if(*first1>*first2){
					iterator tmp = first2;
					__transfer(first1,first2,++tmp);
					first2=tmp;
				}else{
					first1++;
				}
				
			}
			if(first2!=last2) __transfer(last1,first2,last2);	
		}
//list can not use stl sort() which is for randomiterator ,list uses own sort
		void sort(){
			if(m_node->next==m_node||m_node->next->next==m_node) return ;
			list<T,Alloc> carry;
			list<T,Alloc> counter[64];
			int fill = 0;
			while(!empty()){
				int i = 0;
				carry.splice(carry.begin(),*this,begin());
				while(i<fill&&!counter[i].empty()){
					counter[i].merge(carry);
					carry.swap(counter[i++]);
				}
				if(i==fill) fill++;
				counter[i].swap(carry);
			}
			for(int i = 1;i<fill;i++){
				counter[i].merge(counter[i-1]);
			}
			swap(counter[fill-1]);
		}
		void reverse(){
			if(m_node->next==m_node||m_node->next->next==m_node) return;
			iterator cur = ++begin();
			while(cur!=end()){
				iterator old = cur;
				++cur;
				__transfer(begin(),old,cur);
			}
			
		}

	private:
		list_node<T>* __get_node(){return data_allocator::allocate();}
		void __put_node(list_node<T>* p){data_allocator::deallocate(p);}
		list_node<T>* __create_node(const T& val){
			list_node<T>* node = __get_node();
			try{
				construct(&node->val,val);
			}catch(...){
				__put_node(node);
				throw;
			}
		}
		void __destroy_node(list_node<T>* p){destroy(&p->val);__put_node(p);}
		void __empty_initialize(){
			m_node = __get_node();
			m_node->pre = m_node->next = m_node;
		}
		void __fill_initialize(size_type n, const T& val){
			__empty_initialize();
			try{
				while(n--){
					insert(begin(),val);
				}
			}catch(...){
				clear();
				__put_node(m_node);
				throw;
			}
		}
		//template <class Integer>
		//void __assign_dispatch(Integer n, Integer value, __true_type);
		template <class InputIterator>
		void __range_initialize(InputIterator first, InputIterator last){
			__empty_initialize();
			try{
				while(first!=last){
					insert(end(),*first);
					++first;	
				}
			}catch(...){
				clear();
				__put_node(m_node);
				throw;
			}
		}
		void __fill_insert(iterator position, size_type n, const T& value);
		template <class Integer>
		void __insert_dispatch(iterator position, Integer n, Integer value, __true_type);
		template <class InputIterator>
		void __insert_dispatch(iterator position, InputIterator first, InputIterator last, __false_type);
		//move [first,last) to [pos-(last-first),pos ),and [first last) will be empty
		void __transfer(iterator pos, iterator first, iterator last){
			/*if(pos == first ) return ;
			list_node<T>* pos_pre_node = pos.m_node->pre;
			list_node<T>* last_pre_node = last.m_node->pre;
			list_node<T>* first_pre_node = first.m_node->pre;

			last_pre_node->next = pos.m_node;
			first_pre_node->next = last.m_node;

			pos_pre_node->next = first.m_node;
			pos.m_node->next = last_pre_node;
			last.m_node->pre = first_pre_node;

			first.m_node->pre = first_pre_node;*/
			if (pos != last) {
				
				last.m_node->pre->next = pos.m_node;
				first.m_node->pre->next = last.m_node;
				pos.m_node->pre->next = first.m_node;
				
				list_node<T>* tmp = pos.m_node->pre;
				pos.m_node->pre = last.m_node->pre;
				last.m_node->pre = first.m_node->pre;
				first.m_node->pre = tmp;
		
			
			}
		}
	};
	template <class T, class Alloc>
	inline bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
		typedef typename list<T, Alloc>::const_iterator const_iterator;
		const_iterator first1 = lhs.begin();
		const_iterator first2 = rhs.begin();
		const_iterator last1 = lhs.end();
		const_iterator last2 = rhs.end();
		while (first1 != last1 && first2 != last2 && *first1 == *first2) {
			++first1;
			++first2;
		}
		return first1 == last1 && first2 == last2;
	}

	template <class T, class Alloc>
	inline bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
		return jinstl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, class Alloc>
	inline bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template <class T, class Alloc>
	inline bool operator>(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
		return rhs < lhs;
	}

	template <class T, class Alloc>
	inline bool operator<=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
		return !(rhs < lhs);
	}

	template <class T, class Alloc>
	inline bool operator>=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
		return !(lhs < rhs);
	}

}
#endif
