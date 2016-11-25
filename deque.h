#ifndef  JIN_DEQUE_H_
#define  JIN_DEQUE_H_



#include "iterator.h"
#include "alloc.h"
#include"algobase.h"
#include"construct.h"
#include"uninitialized.h"

namespace jinstl {

	// function to decide the buf size
	inline size_t deque_buf_size(size_t n, size_t sz) {
		return n == 0 ? (sz < 512 ? 512 / sz : 1) : n;
	}

	// deque iteraotr
	template <class T, class Ref, class Ptr, size_t BufSiz>
	struct deque_iterator {

		typedef random_access_iterator_tag		iterator_category;
		typedef T					value_type;
		typedef ptrdiff_t	difference_type;
		typedef Ptr	pointer;
		typedef Ref	reference;
		/**************************/
		typedef deque_iterator<T, T&, T*, BufSiz>	iterator;
		typedef deque_iterator<T, const T&, const T*, BufSiz>	const_iterator;
		typedef size_t	size_type;
		typedef T** map_pointer;
		typedef deque_iterator self;
		
		static size_t buffer_size() { return deque_buf_size(BufSiz, sizeof(T)); }

		T*	cur;//cur pointer	
		T*	first;	//first pointer
		T*	last;	//beyond-tail  pointer
		map_pointer node;//main map

		// constructors
		deque_iterator() :cur(0), first(0), last(0), node(0) {}
		deque_iterator(T* curp, map_pointer mapp)
			:cur(curp), first(*mapp), last(*mapp + buffer_size()), node(mapp) {}

		deque_iterator(const iterator& rhs)
			:cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {}

		self& operator=(const iterator& rhs) {
			if (this != &rhs) {
				cur = rhs.cur;
				first = rhs.first;
				last = rhs.last;
				node = rhs.node;
			}
			return *this;
		}

		//jump to another buffer
		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + static_cast<difference_type>(buffer_size());
		}

		reference operator*() const { return *cur; }
		pointer operator->() const { return cur; }

		difference_type operator-(const self& x) const {
			difference_type res = (cur-first)+(node-x.node-1)*buffer_size()+(x.last-x.cur);
			return res;
		}

		self& operator++() {
			cur++;
			if(cur == last){
				set_node(node+1);//update  first last and node	
				cur = first;
			}
			return *this;
		}
		self operator++(int) {
			self res = *this;
			++*this;
			return res;
		}
		self& operator--() {
			if(cur==first){
				set_node(node-1);	
				cur = last;
			}
			--cur;		
			return *this;
		}
		self operator--(int) {
			self res = *this;
			--*this;
			return res;
		}
		self& operator+=(difference_type n) {
			if(cur+n<last){
				cur+=n;
			}else{
				difference_type tmp = n-(last-cur);
				set_node(node+1);
				cur = first + tmp;
			}	
			return *this;
		}
		self operator+(difference_type n) const {
			self res = *this;
			res += n;
			return res;	
		}
		self& operator-=(difference_type n) {
			if(cur - n >=first){
				cur -= n;
			}else{
				difference_type tmp = n-(cur - first);
				set_node(node-1);
				cur = last - tmp-1;
			}	
			return *this;
		}
		self operator-(difference_type n) const {
			self res =*this;
			res-=n;
			return res;	
		}

		reference operator[](difference_type n) const {
			return *(*this+n);
		}

		bool operator==(const self& rhs) const {
			return cur==rhs.cur;
		}
		bool operator<(const self& rhs) const {
			return node==rhs.node&&cur<rhs.cur||node<rhs.node;
		}
		bool operator!=(const self& rhs) const {
			return !(*this == rhs);

		}
		bool operator>(const self& rhs) const { return rhs.cur < cur; }
		bool operator<=(const self& rhs) const { return !(*this>rhs);}
		bool operator>=(const self& rhs) const { return !(*this<rhs); }
	};

	template <class T, class Alloc = alloc, size_t BufSiz = 0>
	class deque {
	public:
		typedef T	value_type;
		typedef	T*	pointer;
		typedef const T*	const_pointer;
		typedef T&	reference;
		typedef	const T&	const_reference;
		typedef size_t	size_type;
		typedef ptrdiff_t	difference_type;

		typedef deque_iterator<T, T&, T*, BufSiz>	iterator;
		typedef deque_iterator<T, const T&, const T*, BufSiz>	const_iterator;
	//	typedef reverse_iterator<const_iterator>	const_reverse_iterator;
	//	typedef reverse_iterator<iterator>	reverse_iterator;

		static size_t buffer_size() { return deque_buf_size(BufSiz, sizeof(T)); }

	public:
		typedef	simple_alloc<T, Alloc>	data_allocator;	//element allocator
		typedef simple_alloc<T*, Alloc>	map_allocator;	//map allocator
		//allocator_type get_allocator() { return allocator_type(); }

	private:
		typedef T**	map_pointer;

		iterator start;	
		iterator finish;	//finish.cur is reserve for probably use
		map_pointer map;	//map is T** pointer to many buf	
		size_type map_size;	//map size

		enum { initial_map_size = 8 };	

	public:
		deque():start(),finish(),map(0),map_size(0) {this->__create_map_and_nodes(0);}
		deque(size_type n, const T& val) {__fill_initialize(n,val); }
		explicit deque(size_type n):start(),finish(),map(0),map_size(0) { __fill_initialize(n,T());}
		template <class InputIterator>
		deque(InputIterator first, InputIterator last){
			this->__create_map_and_nodes(size_type(last - first));
			
			try{
				uninitialized_copy(first,last,start);
			}catch(...){
				this->__destroy_map_and_nodes();
				throw;
			}	

		}

		deque(const deque& rhs){
			this->__create_map_and_nodes(rhs.size());
			try{
				uninitialize_copy(rhs.start,rhs.finish,start);
			}catch(...){
				this->__destroy_map_and_nodes();
				throw;
			}	
		}

		deque& operator=(const deque& rhs){
			
		}

		~deque(){
			destroy(start,finish);
			this->__destroy_map_and_nodes();	
		}

	public:
		iterator begin() {return start; }
		const_iterator begin() const {return start; }
		iterator end() { return finish;}
		const_iterator end() const { return finish;}
		//reverse_iterator rbegin() { }
		//const_reverse_iterator rbegin() const { }
		//reverse_iterator rend() { }
		//const_reverse_iterator rend() const { }

		bool empty() const {return start==finish; }
		size_type size() const {return size_type(finish-start);}
		size_type max_size() const { return size_type(-1);}

		reference front() {return *start; }
		const_reference front() const { return *start;}
		reference back() {return *finish; }
		const_reference back() const {return *finish; }
		reference operator[](size_type n) {return *(start+n); }
		const_reference operator[](size_type n) const {return *(start+n); }
		reference at(size_type n) {return *(start+n); }
		const_reference at(size_type n) const { return *(start+n);}

		//void assign(size_type n, const T& value) {}
		//void assign(size_type n) { }

		//template <class InputIterator>
		//void assign(InputIterator first, InputIterator last){
			
		//}
		iterator insert(iterator pos, const T& val){
			if(pos.cur == start.cur){
				push_front(val);
				return start;
			}else if(pos.cur==finish.cur){
				push_back(val);
				iterator tmp=finish;
				--tmp;
				return tmp;
			}else{
				return __insert_aux(pos,val);
			}	
			
		}
		iterator insert(iterator pos) {insert(pos,T());}
		void insert(iterator pos, size_type n, const T& val){
			if(pos.cur==start.cur){
				iterator new_start = this->__reserve_elements_at_front(n);
				uninitialized_fill(new_start,start,val);
				start = new_start;
			}else if(pos.cur == finish.cur){
				iterator new_finish =this-> __reserve_elements_at_back(n);
				uninitialized_fill(finish,new_finish,val);
				finish = new_finish;
			}else{
				__insert_aux(pos,n,val);
			}
			

		}
		//template <class InputIterator>
		//void insert(iterator position, InputIterator first, InputIterator last){


		//}
		iterator erase(iterator pos){
			iterator tmp = pos;
			tmp++;
			difference_type i = pos -start;
			if(i<(size()/2)){
				copy_backward(start,pos,tmp);
				pop_front();
			}else{
				copy(tmp,finish,pos);
				pop_back();

			}
			return start+i;

		}
		
		iterator erase(iterator pos1,iterator pos2){
			if(start == pos1&&finish==pos2){
				clear();
				return start;
			}
			else{
				difference_type n = pos2-pos1;
				difference_type ele_before = pos1-start;
				if(ele_before<(size()-n)/2){
					copy_backward(start,pos1,pos2);
					iterator new_start = start+n;
					destroy(start,new_start);
					for(T** cur=start.node;cur<new_start.node;cur++){
						data_allocator::deallocate(*cur,buffer_size());
					}
					start=new_start;
				}else{
					copy(pos2,finish,pos1);
					iterator new_finish=finish-n;
					destroy(new_finish+1,finish);
					for(T** cur = new_finish+1;cur<=finish;cur++){
						data_allocator::deallocate(*cur,buffer_size());

					}
					finish = new_finish;
					
				}
				
				return start+ele_before;
			}
			
		}



		void clear(){
			for(T** tmp = start.node+1;tmp<finish.node;tmp++){
				destroy(*tmp,*tmp+buffer_size());
				data_allocator::deallocate(*tmp,buffer_size());
			}
			if(start.node == finish.node) destroy(start.cur,finish.cur);
			else{
				destroy(start.cur,start.finish);
				destroy(finish.first,finish.cur);
				data_allocator::deallocate(finish.first,buffer_size());
			}
			finish =start;

		}
		void push_back(const T& x){
			if(finish.cur == finish.last-1){
				__push_back_aux(x);
			}else{
				construct(finish.cur,x);
				finish.cur++;
				
			}

		}
		void push_front(const T& x){
			if(start.cur==start.first){
				__push_front_aux(x);
			}else{
				construct(--start.cur,x);
				//start.cur--;
			}
		}
		void pop_back(){
			if(finish.cur == finish.first) this->__pop_back_aux();
			else destroy(--finish.cur);
		}
		void pop_front(){
			if(start.cur == start.last-1) this->__pop_front_aux();
			else destroy(start.cur++);
		}
		void resize(size_type new_size, const T& val){
			if(new_size>size()){
				insert(finish,new_size-size(),val);
			}else{
				erase(start+new_size,finish);
			}
		}
		void resize(size_type new_size){ resize(new_size,T());}
		void swap(deque& rhs){
			swap(*this,rhs);
		}

	private:
		T* __allocate_node(){return data_allocator::allocate(buffer_size());}
		void __deallocate_node(T* node){data_allocator::deallocate(node,buffer_size());}
		void __create_map_and_nodes(size_type ele_num){
			size_type node_num = ele_num/buffer_size()+1;
			map_size = max((int)initial_map_size,(int)node_num+2);
			map = map_allocator::allocate(map_size);
			T** map_start = map+(map_size-node_num)/2;//start at the middle
			T** map_finish = map_start+node_num-1;
			T** cur;
			try{
				for(cur = map_start;cur<=map_finish;cur++){
					//*cur = data_allocator::allocate(buffer_size());//allocate every buf
					*cur = __allocate_node();
				}	
				
			}catch(...){
				for(T** tmp=map_start;tmp<=cur;tmp++){
					//data_allocator::deallocate(*tmp,buffer_size());//deallocate every alloced buf
					__deallocate_node(*tmp);
				}
				map_allocator::deallocate(map,map_size);//deallocate the map
				throw;
			}
			start.set_node(map_start);
			finish.set_node(map_finish);
			start.cur = start.first;
			finish.cur=finish.first+ele_num%buffer_size();
		}
		void __destroy_map_and_nodes(){
			for(T** tmp = start.node;tmp<finish.node;tmp++){
				//data_allocator::deallocate(*tmp,buffer_size());//deallocate buf
				__deallocate_node(*tmp);
			}
			map_allocator::deallocate(map,map_size);//deallocate map
		}
			
			
		void __fill_initialize(size_type n, const T& val){
			this->__create_map_and_nodes(n);
			T** tmp;
			try{
				for(tmp = start.node;tmp<finish.node;tmp++){
					uninitialize_fill(*tmp,*tmp+buffer_size(),val);
				}
				uninitialize_fill(finish.first,finish.cur,val);//finish not full
			}catch(...){
				for(T** t = start.node;t<=tmp.node;t++){
					destroy(*t,buffer_size());
				}
				this->__destroy_map_and_nodes();
			}	
		}
		/*template <class InputIterator>
		void __range_initialize(InputIterator, InputIterator, input_iterator_tag);
		template <class ForwardIterator>
		void __range_initialize(ForwardIterator, ForwardIterator, forward_iterator_tag);
		
		template <class Integer>
		void __initialize_dispatch(Integer n, Integer value, __true_type);
		template <class InputIterator>
		void __initialize_dispatch(InputIterator first, InputIterator last, __false_type);
		void __fill_assign(size_type n, const T& value);
		template <class Integer>
		void __assign_dispatch(Integer n, Integer value, __true_type);
		template <class InputIterator>
		void __assign_dispatch(InputIterator first, InputIterator last, __false_type);
		template <class InputIterator>
		void __assign_aux(InputIterator first, InputIterator last, input_iterator_tag);
		template <class ForwardIterator>
		void __assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
		*/
		iterator __insert_aux(iterator pos, const T& val){
			difference_type index= pos-start;
			if(index<size()/2){//insert in left-half
				push_front(front());
				iterator f1 = start;
				++f1;
				iterator f2 = f1;
				++f2;
				pos = start+index;
				iterator pos1=pos;
				++pos1;
				copy(f2,pos1,f1);
				

			}else{
				push_back(back());
				iterator b1 = finish;
				--b1;
				iterator b2=b1;
				--b2;
				pos = start+index;
				copy_backward(pos,b2,b1);
			}
			*pos = val;
			return pos;
			
			
		}
		iterator __insert_aux(iterator pos,size_type n,const T& val){
			difference_type ele_before = pos - start;
			if(ele_before<size()/2){//left half
				iterator new_start = this->__reserve_elements_at_front(n);
				iterator old_start = start;
				pos = start+ele_before;
				try{
					if(ele_before>=n){
						iterator start_n = start+n;
						uninitialized_copy(start,start_n,new_start);
						start = new_start;
						copy(start_n,pos,old_start);
						fill(pos-n,pos,val);
					}else{
						__uninitialized_copy_fill(start,pos,new_start,start,val);
						start = new_start;
						fill(old_start,pos,val);
					}
						
				}catch(...){
					__destroy_nodes_at_front(new_start);
					throw;
					
				}	
				
				
			}else{//right half
				iterator new_finish = this->__reserve_elements_at_back(n);
				iterator old_finish = finish;
				const difference_type ele_after = size()-ele_before;
				pos =finish - ele_after;
				try{
					if(ele_after>n){
						iterator finish_n =finish-n;
						uninitialized_copy(finish_n,finish,finish);
						finish = new_finish;
						copy_backward(pos,finish_n,old_finish);
						fill(pos,pos+n,val);	
					}else{
						__uninitialized_fill_copy(finish,pos+n,val,pos,finish);
						finish = new_finish;
						fill(pos,old_finish,val);
						
					}
				}catch(...){

					__destroy_nodes_at_back(new_finish);
					throw;
				}
			}
		}
	//	void __insert_aux(iteraotr pos,const T* first,const T* last,size_type n){
			
	//	}
	//	template<class ForwardIterator>
	//	void __insert_aux(iterator pos,ForwardIterator first,ForwardIterator last,size_type n){
					
		
	//	}
	//	void __insert_aux(iterator pos,const_iterator first,const_iterator last,size_type n){
			
	//	}
		//when finish.cur = finish.last - 1,then buf which finish.node point to is full	
		void __push_back_aux(const T& x){
			this->__reserve_map_at_back();
			*(finish.node+1) = __allocate_node();
			try{
				construct(finish.cur,x);
				finish.set_node(finish.node+1);
				finish.cur=finish.first;

			}catch(...){
				__deallocate_node(*(finish.node+1));
			}	

		}
		//start.cur == start.first,then is full
		void __push_front_aux(const T& x){
			this->__reserve_map_at_front();
			*(start.node - 1)=__allocate_node();
			try{
				start.set_node(start.node-1);
				
				start.cur = start.last -1;
				construct(start.cur,x);
			}catch(...){
				start.set_node(start.node+1);
				start.cur=start.first;
				__deallocate_node(*(start.node-1));
				throw;
			}
		

		}
		//when finish.cur==finish.first
		void __pop_back_aux(){
			__deallocate_node(finish.first);
			finish.set_node(finish.node-1);
			finish.cur = finish.last - 1;
			destroy(finish.cur);
	
		}
		//when start.cur==start.last-1
		void __pop_front_aux(){
			destroy(start.cur);
			__deallocate_node(start.first);
			start.set_node(start.node+1);
			start.cur = start.first;
			
		}

		

		iterator __reserve_elements_at_front(size_type n){
			size_type vacany = start.cur - start.first;
			if(n>vacany){
				__new_elements_at_front(n-vacany);
			}
			return start-n;

		}
		iterator __reserve_elements_at_back(size_type n){
			size_type vacany = finish.last-finish.cur-1;
			if(n>vacany) __new_elements_at_back(n-vacany);
			return finish + n;

		}

		void __new_elements_at_front(size_type new_elements){
			size_type new_nodes = (new_elements+buffer_size())/buffer_size();
			this->__reserve_map_at_front(new_nodes);
			size_type i;
			try{
				for(i=1;i<=new_nodes;i++)
					*(start.node-i)=__allocate_node();
			}catch(...){
				for(size_type j=1;j<i;++j)
					__deallocate_node(*(start.node-j));
				throw;
			}


		}
		void __new_elements_at_back(size_type new_elements){
			size_type new_nodes = (new_elements+buffer_size())/buffer_size();
			this->__reserve_map_at_back(new_nodes);
			size_type i;
			try{
				for(i=1;i<=new_nodes;i++)
					*(finish.node+i)=__allocate_node();//allocate buf
			}catch(...){
				for(size_type j=1;j<i;++j)
					__deallocate_node(*(finish.node+j));//deallocate buf
				throw;
			}
			


		}

		void __destroy_nodes_at_front(iterator before_start){
			for(T** tmp = before_start.node;tmp<start.node;tmp++){
				__deallocate_node(*tmp);
			}

		}

		void __destroy_nodes_at_back(iterator after_finish){
			for(T** tmp = after_finish.node;tmp>finish.node;tmp--){
				__deallocate_node(*tmp);
			}
			
		}
		
		void __reserve_map_at_back(size_type nodes_to_add =1){
			if(nodes_to_add+1>map_size-(finish.node-map)){
				__reallocate_map(nodes_to_add,false);
			}

		}
	
		void __reserve_map_at_front(size_type nodes_to_add =1){
			if(nodes_to_add>start.node-map)
				__reallocate_map(nodes_to_add,true);
		}
		
		void __reallocate_map(size_type nodes_to_add, bool add_at_front){
			size_type old_num_nodes = finish.node - start.node + 1;
			size_type new_num_nodes = old_num_nodes + nodes_to_add;
			T** new_nstart;
			if(map_size>2*new_num_nodes){
				new_nstart = map +(map_size-new_num_nodes)/2+(add_at_front?nodes_to_add:0);
				if(new_nstart<start.node){
					copy(start.node,finish.node+1,new_nstart);
				}else{
					copy_backward(start.node,finish.node+1,new_nstart+old_num_nodes);
				}
			}else{
				size_type new_map_size = map_size+jinstl::max((int)map_size,(int)nodes_to_add)+2;
				T** new_map = map_allocator::allocate(new_map_size);
				new_nstart=new_map+(new_map_size-new_num_nodes)/2+(add_at_front?nodes_to_add:0);
				copy(start.node,finish.node+1,new_nstart);
				map_allocator::deallocate(map,map_size);
				map=new_map;
				map_size = new_map_size;	

			}
			start.set_node(new_nstart);//update node first last in iterator
			finish.set_node(new_nstart+old_num_nodes-1);

		}
	};

	/**********************************************************************************/
	template<class T,class Alloc,size_t bufsize>
	inline bool operator==(const deque<T,Alloc,bufsize>& v1,const deque<T,Alloc,bufsize>&v2){
		return v1.size()==v2.size() && equal(v1.begin(),v1.end(),v2.begin());
	}		
	template<class T,class Alloc,size_t bufsize>
	inline bool operator<(const deque<T,Alloc,bufsize>&v1,const deque<T,Alloc,bufsize>&v2){
		return lexicographical_compare(v1.begin(),v1.end(),v2.begin(),v2.end());
	}
	template<class T>
	void swap(deque<T>&q1,deque<T>&q2){
		swap(q1.start,q2.start);
		swap(q1.finish,q2.finish);
		swap(q1.map,q2.map);
		swap(q1.map_size,q2.map_size);
	}
	
}
#endif 

