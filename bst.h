/*bsh.h*/

//
// Binary search tree
//

#pragma once

#include <iostream>
#include <algorithm>  // std::max

using namespace std;

template<typename TKey, typename TValue>
class binarysearchtree
{
private:
  struct NODE
  {
    TKey  Key;
	TValue Value;
    NODE* Left;
    NODE* Right;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)

  //
  // _height does the actual work of computing height, recursively.
  //
  int _height(NODE* cur)
  {
    if (cur == nullptr)
      return -1;
    else
    {
      int heightL = _height(cur->Left);
      int heightR = _height(cur->Right);

      return 1 + std::max(heightL, heightR);
    }
  }

  // 
  // _inorder does the actual inorder traversal and output 
  // to console.  Each key and value is outputted to the 
  // console followed by ", ", including the last key and value
  //
  void _inorder(NODE* cur)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder(cur->Left);
      cout << cur->Key << ", " << cur->Value << endl;
      _inorder(cur->Right);
    }
  }

public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  binarysearchtree()
  {
    Root = nullptr;
    Size = 0;
  }
  
  //copy constructor helper function
  void helperConstructor(NODE* cur) {
    if(cur == nullptr) {
	  return;
	}
	else {
	  insert(cur->Key, cur->Value);
	  helperConstructor(cur->Left);
	  helperConstructor(cur->Right);
	}
  }
  
  //
  // copy constructor:
  //
  binarysearchtree(binarysearchtree& other) //return type?
  {
    Root = nullptr;
	Size = 0;
	NODE* ptr = other.Root;
    helperConstructor(ptr);
  }
  
  void _binarysearchtree(NODE* cur) {
	if(cur == nullptr) {
		return;
	}
	else {
		_binarysearchtree(cur->Left);
		_binarysearchtree(cur->Right);
		delete cur;
	}
  }

  //
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~binarysearchtree()
  {
    _binarysearchtree(Root); 
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  int size()
  {
    return Size;
  }

  //
  // height
  //
  // Computes and returns height of tree; height of an empty tree is
  // defined as -1.
  //
  int height()
  {
    return _height(Root);
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning a pointer
  // to the value. If key is not found, nullptr is returned.
  //
  TValue* search(TKey key)
  {
    NODE* cur = Root;

    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
        return &(cur->Value);

      if (key < cur->Key)  // search left:
      {
        cur = cur->Left;
      }
      else
      {
        cur = cur->Right;
      }
    }//while  

    // if get here, not found
    return nullptr;
  }

  //
  // insert
  //
  // Inserts the given key and value into the tree; if the key has already been inserted then
  // then the function returns without changing the tree.
  //
  void insert(TKey key, TValue value)
  {
    NODE* prev = nullptr;
    NODE* cur = Root;

    //
    // 1. Search to see if tree already contains key:
    //
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
        return;

      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else                // search right:
      {
        prev = cur;
        cur = cur->Right;
      }
    }//while

    //
    // 2. if we get here, key is not in tree, so allocate
    // a new node to insert:
    // 
    NODE* newNode;
    newNode = new NODE();
    newNode->Key = key;
	newNode->Value = value;
    newNode->Left = nullptr;
    newNode->Right = nullptr;

    //
    // 3. link in the new node:
    //
    // NOTE: cur is null, and prev denotes node where
    // we fell out of the tree.  if prev is null, then
    // the tree is empty and the Root pointer needs 
    // to be updated.
    //
    if (prev == nullptr)
      Root = newNode;
    else if (key < prev->Key)
      prev->Left = newNode;
    else
      prev->Right = newNode;

    // 
    // 4. update size and we're done:
    //
    Size++;
  }

  //
  // inorder:
  //
  // Performs an inorder traversal of the tree, outputting
  // the keys to the console.
  //
  void inorder()
  {
    cout << "Inorder: ";

    _inorder(Root);

    cout << endl;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
	_binarysearchtree(Root);
    //
    // Re-initialize root pointer and size to "empty":
    //
    Root = nullptr;
    Size = 0;
	
	
    
//     //
//     // Intentional pointer error:
//     //
//     Root->Key = 123;
  }

};
