#include <iostream>
#include <vector>
#include <exception>

#ifndef _HASH_MAP_HPP_
#define _HASH_MAP_HPP_

#define START_CAPACITY 16
#define START_ELEMENTS_NUM 0
#define ZERO 0

#define VECTOR_SIZE_ERROR_MSG "Error: the vectors are not the same size."
#define FIRST_INDEX 0
#define LOWEST_LOADING_FACTOR 3/4




using std::vector;
using std::pair;


template <typename KeyT, typename ValueT>
class HashMap{
public:
    class ConstFwIterator;
    typedef ConstFwIterator const_iterator;

    //~~~~HashMap Methods~~~~//
    /** default constructor*/
    HashMap(): _elements_num(START_ELEMENTS_NUM), _capacity(START_CAPACITY),
    _hashmap(new vector<pair<KeyT, ValueT>>[START_CAPACITY]){}

    /** constructor 1 , receives two vectors, KeyT and ValueT and saves them to the hash map*/
    HashMap(vector<KeyT>& keys_vec, vector<ValueT>& values_vec): _elements_num(START_ELEMENTS_NUM),
    _capacity(START_CAPACITY), _hashmap(new vector<pair<KeyT, ValueT>>[START_CAPACITY])
    {
        if(keys_vec.size() != values_vec.size())
        {
            throw std::length_error(VECTOR_SIZE_ERROR_MSG);
        }

        //todo:check if key is in the map

        //todo:insert into the hash map
    }

    /**Copy constructor*/
    HashMap(const HashMap* hashmap): _elements_num(hashmap->_elements_num),
                                      _capacity(hashmap->_capacity),
    _hashmap(new vector<pair<KeyT, ValueT>>[hashmap->_capacity])
    {

        //todo:insert into the this->hashmap

    }

    /** destructor */
    ~HashMap()
    {
        delete[] _hashmap;
        _hashmap = nullptr;
    }


    bool insert(const KeyT& key, const ValueT& value) const
    {
        //check if key is in the hashmap - if it is return False
        if(this->contains_key(key))
        {
          return false;
        }

        //else add the key and value to the hashmap
        //first check hash value
        int hash_value = hash(key);
        //check if the loading factor is bigger then the lowest limit of the
        // loading factor. If it is bigger -> rehash
        if(this->_hashmap.get_load_factor() > LOWEST_LOADING_FACTOR)
        {
            //todo rehash
        }
        else
        {
            // insert key and value into the hashmap in the bucket with the
            // index of the hash value
            this->_hashmap[hash_value].push_back(pair<KeyT, ValueT>(key,
                                                                    value));
            this->_elements_num++;

        }
        
    }

    bool contains_key(KeyT& key) const
    {
        int hash_value = hash(key);
        vector<pair<KeyT, ValueT>> vec = this->_hashmap[hash_value];

        for(int i=0; i<vec.size(); i++)
        {
          if(vec[i].first == key)
          {
            return true;
          }
        }
        return false;
    }

    /** Returns the size of the hash map*/
    int size() const
    {
        return _elements_num;
    }

    /** Returns the capacity of the hash map*/
    int capacity() const
    {
        return _capacity;
    }
    /** Returns True if the hash map is empty*/

    bool empty() const
    {
        return (_elements_num == 0);
    }

    /** Returns loading factor of the hash map*/
    double get_load_factor() const
    {
        return (double) (_elements_num / _capacity);
    }

    //~~~~HashMap Operators~~~~//
    



    bool operator==(const HashMap &rhs) const {
      if(this->_hashmap.size() != rhs._hashmap.size())
      {
          return false;
      }
      //run on the buckets
      for (int i = 0; i < this->_hashmap.size(); ++i) {
          for (int j = 0; j < this->_hashmap[i].size(); ++j) {
              if( this->_hashmap[i][j].first)

          }
      }
    }

    //~~~~Iterator methods~~~~//

    const_iterator cbegin () const
    {
        return ConstFwIterator(this->_hashmap, FIRST_INDEX, FIRST_INDEX);
    }

    const_iterator cend () const
    {
        return ConstFwIterator(this->_hashmap, this->_capacity, FIRST_INDEX);
    }

    const_iterator begin () const
    {
        return cbegin();
    }

    const_iterator end () const
    {
        return cend();
    }


    //~~~~Const Iterator~~~~~//
    /** const forward iterator class - the iterator iterates over the vector of pairs of KeyT and ValueT*/
    class ConstFwIterator
    {
    public:
      //~~~~iterator traits~~~~//
      typedef std::forward_iterator_tag iterator_category;
      typedef pair<KeyT, ValueT> value_type;
      typedef pair<KeyT, ValueT>& reference;
      typedef pair<KeyT, ValueT>* pointer;
      typedef std::ptrdiff_t difference_type;

      //~~~~iterator methods~~~~//

      /**pre increment-
         * We need to check if the iterator is in the end a vector, if so then we need to go to the next index of the
         * hash map.
         * We also need to check if the iterator is at the end of the hash map*/
      ConstFwIterator& operator++ ()
      {
        //check if the pair index exceeds bucket size
        if((pair_index_ + 1) > (map_ptr_[bucket_index_].size() - 1))
        {
          //if it exceeds bucket size - go to the next not empty bucket
          find_next_not_empty_pair(map_ptr_, bucket_index_);
        }
        else // pair doesn't exceed bucket size
        {
          current_iter_ = map_ptr_[bucket_index_][pair_index_ + 1];
          pair_index_ = pair_index_ + 1;
        }
        //todo: check what type to return
        return current_iter_;
      }

      /**post increment*/
      ConstFwIterator operator++ (int)
      {
        pair<KeyT, ValueT> tmp = current_iter_;
        ++current_iter_;
        return tmp;
      }


      bool operator== (const ConstFwIterator &rhs) const
      {
        //check if key and value are the same
        if((current_iter_.first == rhs->first) && (current_iter_.second == rhs->second))
        {
          return true;
        }
        return false;
      }


      bool operator!= (const ConstFwIterator &rhs) const
      {
        //check if key and value are the same
        if((current_iter_.first != rhs->first) || (current_iter_.second != rhs->second))
        {
          return true;
        }
        return false;
      }

      //returns a reference to the element in the container that the iterator is currently pointing to.
      reference operator* () const
      {
        return current_iter_;
      }

      //returns a pointer to the element in the container that the iterator is currently pointing to.
      pointer operator-> () const
      {
        return current_iter_;
      }


    private:

      void find_next_not_empty_pair(const vector<pair<KeyT, ValueT>> *map_ptr, int start_bucket_index = 0) {// find the first bucket that is not empty
        for(int i=start_bucket_index; i<(map_ptr->size()); i++)
        {
          if(!map_ptr[i].empty())
          {
            // take the first element of the bucket.
            current_iter_ = map_ptr[i][0];
            bucket_index_ = i;
            pair_index_ = 0;
            break;
          }
        }
      }

      /** default constructor*/
      ConstFwIterator(): map_ptr_(nullptr), current_iter_(nullptr){}

      /**constructor*/
      ConstFwIterator(const vector<pair<KeyT, ValueT>>* map_pointer, int bucket_index, int pair_index): map_ptr_(map_pointer),
                                                                                                         bucket_index_(bucket_index), pair_index_(pair_index)
      {
        // if map is not empty
        if(!map_ptr_->empty())
        {
          //check if pair in the bucket index + pair index is empty
          if ((map_ptr_[bucket_index_].empty()) || ((map_ptr_[bucket_index_].size() - 1))< pair_index_)
          {
            find_first_not_empty_pair(map_ptr_);
          }
          else // if the indices are ok - set iterator to the given pair in those indices
          {
            current_iter_ = map_ptr_[bucket_index_][pair_index_];
          }
        }
        // if map is empty
        current_iter_ = nullptr;

      }

      vector<pair<KeyT, ValueT>>* map_ptr_;
      pair<KeyT, ValueT> current_iter_;
      int bucket_index_;
      int pair_index_;
    };



private:
    vector<pair<KeyT, ValueT>>* _hashmap;
    int _elements_num;
    int _capacity;


    long hash(KeyT& key)
    {
        //transforms the KeyT object into a integer type
        long v = std::hash<KeyT>(key);
        //formula
        long hash_value = v & (_capacity - 1);

        return hash_value;
    }




};





#endif //_HASH_MAP_HPP_