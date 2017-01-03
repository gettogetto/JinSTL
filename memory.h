#ifndef JIN_MEMORY_H
#define JIN_MEMORY_H
#include<functional>
#include<atomic>
#include<cstddef>
using std::size_t;
using std::nullptr_t;

namespace jinstl{
template<class T>
struct deleter{
    void operator()(T* ptr){
        if(ptr) delete ptr;
    }
};

template<class T>
class ref_t{
public:
    using Deleter = std::function<void(T*)>;
    std::atomic<size_t> _count;
    T* _data;
    Deleter _deleter;

    explicit ref_t(T* p=nullptr,Deleter func=deleter<T>())
        :_data(p),_deleter(func)
    {
        if(_data!=nullptr){
            _count=1;
        }else{
            _count=0;
        }
    }
    ref_t(const ref_t& rhs)=delete;
    ref_t& operator=(const ref_t& rhs)=delete;
    ~ref_t(){
        --_count;
        if(_count==0){
            _deleter(_data);
        }
    }
    size_t count(){
        return _count.load();
    }
    T* data(){
        return _data;
    }
    ref_t& operator++(){
        ++_count;
        return *this;
    }
    ref_t operator++(int){
        auto tmp=*this;
        ++*this;
        return tmp;
    }
    ref_t& operator--(){
        --_count;
        return *this;
    }
    ref_t operator--(int){
        auto tmp=*this;
        --*this;
        return tmp;
    }

    friend bool operator==(const ref_t& lhs,const ref_t& rhs){
        return lhs._data==rhs._data;
    }

    friend bool operator!=(const ref_t& lhs,const ref_t& rhs){
        return !(lhs==rhs);
    }
};

template<class T>
class shared_ptr{
public:
    explicit shared_ptr(T* data=nullptr):_ref(new ref_t<T>(data)){

    }
    ~shared_ptr(){
        _decrease_ref();
    }

    shared_ptr(const shared_ptr& rhs){
        _increase_ref(rhs._ref);
    }
    shared_ptr& operator=(const shared_ptr& rhs){
        if(this==&rhs) return *this;
        _decrease_ref();
        _increase_ref(rhs._ref);
    }
    size_t use_count(){
        return _ref->count();
    }

    T* get(){
        return _ref->data();
    }
    const T* get()const{
        return _ref->data();
    }


    T* operator->(){
        return _ref->data();
    }
    T& operator*(){
        return *_ref->data();
    }

    const T* operator->()const {
        return _ref->data();
    }
    const T& operator*()const{
        return *_ref->data();
    }

    operator bool()const{
        return _ref->data()==nullptr;
    }

private:
    ref_t<T>* _ref;

    void _increase_ref(ref_t<T> *r){
        _ref=r;
        ++(*_ref);
    }
    void _decrease_ref(){
        if(_ref->data()){
            --(*_ref);
            if(use_count()==0){
                delete _ref;
            }
        }
    }

    friend bool operator==(const shared_ptr&lhs,const shared_ptr&rhs){
        return lhs.get()==rhs.get();
    }
    friend bool operator!=(const shared_ptr&lhs,const shared_ptr&rhs){
        return !(lhs==rhs);
    }
    friend bool operator==(const shared_ptr&lhs,nullptr_t n){
        return lhs.get()==n;
    }
    friend bool operator!=(const shared_ptr&lhs,nullptr_t n){
        return !(lhs==n);
    }
    friend bool operator==(nullptr_t n,const shared_ptr&lhs){
        return lhs.get()==n;
    }
    friend bool operator!=(nullptr_t n,const shared_ptr&lhs){
        return !(lhs==n);
    }
    template<class ...Args>
    friend shared_ptr make_shared(Args...args){
        return shared_ptr(new T(std::forward<Args>(args)...));
    }

};


}

#endif
