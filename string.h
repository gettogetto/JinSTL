#ifndef JIN_STRING_H_
#define JIN_STRING_H_
#include"alloc.h"
#include"iterator.h"
#include"uninitialized.h"
#include"construct.h"
#include"algobase.h"
#include"stddef.h"
#include<stdio.h>
namespace jinstl{
//template<class Alloc=alloc>
class string{
public:
    typedef char* iterator;
    typedef char& reference;
    typedef simple_alloc<char,alloc> data_allocator;
    typedef const char* const_iterator;
    typedef const char& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference;
    const static size_t npos = -1;
private:
    char* _start;
    char* _finish;
    char* _end_of_storage;
public:
    string():_start(0),_finish(0),_end_of_storage(0){}
    string(const string& str){
        allocate_and_copy(str.length(),str.begin(),str.end());
    }

    template<class InputIterator>
    string(InputIterator first,InputIterator last){
        size_type n=0;
        n=distance(first,last);
        _start = allocate_and_copy(n,first,last);
        _finish = _start+n;
        _end_of_storage=_finish;
    }
    //const param is not overload
    /*template<class InputIterator>
    string(const InputIterator first,const InputIterator last){
        size_type n=0;
        n=distance(first,last);
        _start = allocate_and_copy(n,first,last);
        _finish = _start+n;
        _end_of_storage=_finish;
    }*/
    string(const string& str,size_type pos,size_type n=npos){
        if(n==npos){
            n=str.size()-pos;
        }
        allocate_and_copy(n,str.begin()+pos,str.begin()+pos+n);
    }

    string(const char* s,size_type n){

        _start=allocate_and_copy(n,s,s+n);
        _finish=_start+n;
        _end_of_storage=_finish;
    }

    string(const char* s){

        _start=allocate_and_copy(strlen(s),s,s+strlen(s));
        _finish=_start+strlen(s);
        _end_of_storage=_finish;
    }

    string(size_type n,char c){
        fill_initialize(n,c);
    }


    ~string(){
        destroy(begin(),end());
        deallocate();
    }

    string& operator+=(const string& str){
        return append(str);
    }

    string& operator+=(const char* s){
        return append(s);
    }

    string& operator+=(const char c){
        return append(c);
    }

    iterator begin()const{return _start;}
    iterator end()const{return _finish;}

    const_iterator cbegin()const{return _start;}
    const_iterator cend()const{return _finish;}
    char& back(){return *(_finish-1);}
    const char& back() const{return *(_finish-1);}
    const char& front() const {return *(_start);}
    char& front(){return *(_start);}
    bool empty()const{return _start==_finish;}
    void clear(){
        destroy(_start,_finish);//do nothing because char
        _start=_finish;
    }

    size_type size()const{
        return _finish-_start;
    }
    size_type capacity()const{
        return _end_of_storage-_start;
    }
    size_type length()const{
        return size();
    }
    void shrink_to_fit(){
        char* new_start = static_cast<char*>(data_allocator::allocate(size()));
        char* new_finish = static_cast<char*>(uninitialized_copy(_start,_finish,new_start));
        data_allocator::deallocate(_start,capacity());
        _start = new_start;
        _finish = new_finish;
        _end_of_storage = _finish;
    }
    char& operator[](size_t i){
        return *(_start+i);
    }

    const char& operator[](size_t i) const{
        return *(_start+i);
    }

    void resize(size_type n){
        resize(n,char());
    }
    void resize(size_type n,char c){
        if(n<size()){
                erase(_start+n,_finish);
        }
        else{

                insert(_start,n-size(),c);
        }
    }


    void reserve(size_type n){
        if(n<capacity()) return;
        size_type oldsize = size();
        iterator newstart = allocate_and_copy(n,_start,_finish);
        destroy(_start,_finish);
        deallocate();
        _start=newstart;
        _finish=_start+oldsize;
        _end_of_storage=_start + n;
    }


    string& operator= (const string& str);
    string& operator= (string&& str);
    string& operator= (const char* s);
    string& operator= (char c);
    void push_back(char c){ insert(end(), c); }

    string& insert(size_t pos, const string& str){
        insert(begin()+pos,str.cbegin(),str.cend());
        return *this;
    }

    string& insert(size_t pos, const string& str, size_t subpos, size_t sublen = npos){
        if(sublen==npos){
            sublen=str.size()-subpos;
        }
        insert(begin()+pos,str.begin()+subpos,str.begin()+sublen);
        return *this;
    }

    string& insert(size_t pos, const char* s){
        insert(begin()+pos,s,s+strlen(s));
        return *this;
    }

    string& insert(size_t pos, const char* s, size_t n){
        insert(begin()+pos,s,s+n);
        return *this;
    }

    string& insert(size_t pos, size_t n, char c){
        insert(_start+pos,n,c);
        return *this;
    }


    iterator insert(iterator p, size_t n, char c){
        size_type left=capacity()-size();
        if(left>=n){
            copy_backward(p,end(),p+n);
            uninitialized_fill_n(p,n,c);
            _finish+=n;
            return p+n;
        }else{
            return insert_aux_fill_n(p,n,c);
        }
    }

    iterator insert(iterator p, const char c){
        insert_aux(p,c);
    }

    template <class InputIterator>
    iterator insert(iterator p, InputIterator first, InputIterator last){
        size_type left=capacity()-size();
        size_type n=distance(first,last);
        if(left>=n){
            copy(p,end(),p+n);
            uninitialized_copy(first,last,p);
            _finish+=n;
            return p+n;
        }else{
            return insert_aux_copy(p,first,last);
        }


    }

    string& append(const string& str){
        insert(size(),str);
        return *this;
    }

    string& append(const string& str, size_t subpos, size_t sublen = npos){
        insert(size(),str,subpos,sublen);
        return *this;
    }

    string& append(const char* s){
        insert(size(),s);
        return *this;
    }

    string& append(const char* s, size_t n){
        insert(size(),s,n);
        return *this;
    }

    string& append(size_t n, const char c){
        insert(end(),n,c);
        return *this;
    }
    string& append(const char c){
        insert(end(),c);
        return *this;
    }
    template <class InputIterator>
    string& append(InputIterator first, InputIterator last){
        insert(end(),first,last);
        return *this;
    }






    void pop_back(){ erase(end() - 1, end()); }
    string& erase(size_t pos = 0, size_t len = npos){
        if(len==npos){
            len=size()-pos;
        }
        erase(begin()+pos,begin()+pos+len);
        return *this;
    }


    iterator erase(iterator pos){
            if(pos!=_finish){
                    copy(pos+1,_finish,pos);
            }
            destroy(--_finish);

            return pos;
    }
    iterator erase(iterator first,iterator last){

            iterator tmp = copy(last,_finish,first);
            destroy(tmp,_finish);
            _finish -= last - first;
            return first;
    }

    string& replace(size_t pos, size_t len, const string& str){
        iterator p=erase(begin()+pos,begin()+pos+len);
        insert(p,str.begin(),str.end());
        return *this;
    }

    string& replace(iterator i1, iterator i2, const string& str){
        iterator p=erase(i1,i2);
        insert(p,str.begin(),str.end());
        return *this;
    }

    string& replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen = npos){
        iterator p=erase(begin()+pos,begin()+pos+len);
        insert(p-begin(),str,subpos,sublen);
        return *this;
    }

    string& replace(size_t pos, size_t len, const char* s){
        iterator p=erase(begin()+pos,begin()+pos+len);
        insert(p-begin(),s);
        return *this;
    }

    string& replace(iterator i1, iterator i2, const char* s){
        iterator p=erase(i1,i2);
        insert(p-begin(),s);
        return *this;
    }

    string& replace(size_t pos, size_t len, const char* s, size_t n){
        iterator p=erase(begin()+pos,begin()+pos+len);
        insert(p-begin(),s,n);
        return *this;
    }

    string& replace(iterator i1, iterator i2, const char* s, size_t n){
        iterator p=erase(i1,i2);
        insert(p-begin(),s,n);
        return *this;
    }

    string& replace(size_t pos, size_t len, size_t n, char c){
        iterator p=erase(begin()+pos,begin()+pos+len);
        insert(p,n,c);
        return *this;
    }

    string& replace(iterator i1, iterator i2, size_t n, char c){
        iterator p=erase(i1,i2);
        insert(p,n,c);
        return *this;
    }

    template <class InputIterator>
    string& replace(iterator i1, iterator i2, InputIterator first, InputIterator last){
           iterator p = erase(i1,i2);
           insert(p,first,last);
           return *this;
    }

    void swap(string& str){
            jinstl::swap(_start, str._start);
            jinstl::swap(_finish, str._finish);
            jinstl::swap(_end_of_storage, str._end_of_storage);
    }



    string substr(size_t pos = 0, size_t len = npos) const{
        if(len==npos){
            len=size()-pos;
        }
        return string(begin() + pos, begin() + pos + len);
    }

    int compare(const string& str) const{
            return compare(0, size(), str, 0, str.size());
    }
    int compare(size_t pos, size_t len, const string& str) const{
            return compare(pos, len, str, 0, str.size());
    }

    int compare(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen) const{
            return compare_aux(pos, len, str.begin(), subpos, sublen);
    }
    int compare(const char* s) const{
            return compare(0, size(), s, strlen(s));
    }
    int compare(size_t pos, size_t len, const char* s) const{
            return compare(pos, len, s, strlen(s));
    }
    int compare(size_t pos, size_t len, const char* s, size_t n) const{
            return compare_aux(pos, len, s, 0, n);
    }
private:
    void deallocate(){
            if(_start){
                    data_allocator::deallocate(_start,_end_of_storage-_start);
            }
    }
    void fill_initialize(size_type n,const char val){
            _start=allocate_and_fill(n,val);
            _finish = _start + n;
            _end_of_storage = _finish;
    }
    iterator allocate_and_copy(size_type n,const_iterator first,const_iterator last){
            iterator res = data_allocator::allocate(n);
            try{
                    uninitialized_copy(first,last,res);
                    return res;
            }catch(...){
                    data_allocator::deallocate(res,n);
                    throw;
            }
    }
    iterator allocate_and_fill(size_type n,const char val){
            iterator res = data_allocator::allocate(n);
            try{
                    uninitialized_fill_n(res,n,val);
                    return res;
            }catch(...){
                    data_allocator::deallocate(res,n);
                    throw;
            }
    }
    void insert_aux(iterator pos,const char val){
            if(_finish!=_end_of_storage){//has space
                    construct(_finish,*(_finish-1));
                    ++_finish;
                    copy_backward(pos,_finish-2,_finish-1);
                    *pos = val;
            }else{

                    size_type old_element_count = size();
                    size_type new_capacity = old_element_count?2*old_element_count:1;
                    iterator newstart = data_allocator::allocate(new_capacity);
                    //every step should record the newfinish,if exception happend,destroy the constructed objs
                    iterator newfinish = newstart;
                    try{
                            newfinish = uninitialized_copy(_start,pos,newstart);
                            construct(newfinish,val);
                            ++newfinish;
                            newfinish = uninitialized_copy(pos,_finish,newfinish);
                    }catch(...){
                            destroy(newstart,newfinish);
                            data_allocator::deallocate(newstart,new_capacity);
                            throw;
                    }
                    destroy(_start,_finish);
                    deallocate();
                    _start = newstart;
                    _finish = newfinish;
                    _end_of_storage = _start + new_capacity;
            }
    }
    iterator insert_aux_fill_n(iterator pos,size_type n,char c){
        size_type old_capacity=_end_of_storage-_start;
        size_type new_capacity=old_capacity!=0?old_capacity+max(old_capacity,n):n;
        iterator new_start=data_allocator::allocate(new_capacity);
        iterator new_finish=uninitialized_copy(_start,pos,new_start);

        new_finish=uninitialized_fill_n(new_finish,n,c);
        iterator res =new_finish;
        new_finish=uninitialized_copy(pos,_finish,new_finish);

        destroy(_start,_finish);
        deallocate();

        _start = new_start;
        _finish=new_finish;
        _end_of_storage=_start+new_capacity;
        return res;

    }
    template<class InputIterator>
    iterator insert_aux_copy(iterator pos,InputIterator first,InputIterator last){
        size_type old_capacity=_end_of_storage-_start;
        size_type n=distance(first,last);
        size_type new_capacity=old_capacity!=0?old_capacity+max(old_capacity,n):n;
        iterator new_start=data_allocator::allocate(new_capacity);
        iterator new_finish=uninitialized_copy(_start,pos,new_start);

        new_finish=uninitialized_copy(first,last,new_finish);
        iterator res =new_finish;
        new_finish=uninitialized_copy(pos,_finish,new_finish);

        destroy(_start,_finish);
        deallocate();

        _start = new_start;
        _finish=new_finish;
        _end_of_storage=_start+new_capacity;
        return res;
    }
    int compare_aux(size_t pos, size_t len, const_iterator cit, size_t subpos, size_t sublen)const{
            size_t i, j;
            for (i = 0, j = 0; i != len && j != sublen; ++i, ++j){
                    if ((*this)[pos + i] < cit[subpos + j])
                            return -1;
                    else if ((*this)[pos + i] > cit[subpos + j])
                            return 1;
            }
            if (i == len && j == sublen)
                    return 0;
            else if (i == len)
                    return -1;
            else
                    return 1;
    }

};

template<class Iterator1, class Iterator2>
bool lessEqual(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2){
        for (; first1 != last1 && first2 != last2; ++first1, ++first2){
                if (*first1 < *first2)
                        return true;
                else if (*first1 > *first2)
                        return false;
        }
        if ((first1 == last1 && first2 == last2)|| (first1 == last1))
                return true;
        else
                return false;
}

bool operator<= (const string& lhs, const string& rhs){
    return lessEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
bool operator<= (const char*   lhs, const string& rhs){
    return lessEqual(lhs, lhs + strlen(lhs), rhs.begin(), rhs.end());
}
bool operator<= (const string& lhs, const char*   rhs){
    return lessEqual(lhs.begin(), lhs.end(), rhs, rhs + strlen(rhs));
}
bool operator>  (const string& lhs, const string& rhs){
    return !(lhs <= rhs);
}
bool operator>  (const char*   lhs, const string& rhs){
    return !(lhs <= rhs);
}
bool operator>  (const string& lhs, const char*   rhs){
    return !(lhs <= rhs);
}
template<class Iterator1, class Iterator2>
bool greaterEqual(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2){
        for (; first1 != last1 && first2 != last2; ++first1, ++first2){
                if (*first1 > *first2)
                        return true;
                else if (*first1 < *first2)
                        return false;
        }
        if ((first1 == last1 && first2 == last2) || (first2 == last2))
                return true;
        else
                return false;
}

bool operator>= (const string& lhs, const string& rhs){
    return greaterEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
bool operator>= (const char*   lhs, const string& rhs){
    return greaterEqual(lhs, lhs + strlen(lhs), rhs.begin(), rhs.end());
}
bool operator>= (const string& lhs, const char*   rhs){
    return greaterEqual(lhs.begin(), lhs.end(), rhs, rhs + strlen(rhs));
}

std::ostream& operator <<(std::ostream& os, const string&str){
        for (const char ch : str){
                os << ch;
        }
        return os;
}
std::istream& operator >> (std::istream& is, string& str){
        char ch;
        string::size_type oldSize = str.size(), index = 0;
        bool hasPrevBlank = false;
        while (is.get(ch)){
                if (isblank(ch) || ch == '\n')
                        hasPrevBlank = true;
                else
                        break;
        }
        is.putback(ch);
        str.clear();
        while (is.get(ch)){
                if (ch != EOF && !isblank(ch) && ch != '\n'){
                        str.push_back(ch);
                }
                else
                        break;
        }
        return is;
}
}

#endif
