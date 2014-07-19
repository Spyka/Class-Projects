//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///    
///    CS 310
///    Programming Assignment #1 - Stack.java
///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

package data_structures;

import java.util.Iterator;

public class Stack<E> implements Iterable<E>
{
    ListADT<E> stackList;
    
    public Stack()
    {
        stackList = new LinkedListDS<E>();
    }
    
// inserts the object obj into the stack
    public void push(E obj) 
    {
        stackList.addFirst(obj);
    }
    
    // pops and returns the element on the top of the stack    
    public E pop() 
    {
        return stackList.removeFirst();
    }
    // returns the number of elements currently in the stack
    public int size() 
    {
        return stackList.size();
    }
    // return true if the stack is empty, otherwise false
    public boolean isEmpty()    
    {
        return stackList.isEmpty();
    }    
    
    // returns but does not remove the element on the top of the stack    
    public E peek() 
    {
        return stackList.peekFirst();
    }
    // returns true if the object obj is in the stack,
    // otherwise false  
    public boolean contains(E obj) 
    {
        return stackList.contains(obj);
    }
    // returns the stack to an empty state    
    public void makeEmpty() 
    {
        stackList.makeEmpty();
    }
    // removes the Object obj if it is in the stack and
    // returns true, otherwise returns false.
    public boolean remove(E obj)    
    {
        return stackList.remove(obj);
    }
    // returns a iterator of the elements in the stack.  The elements
    // must be in the same sequence as pop() would return them.    
    public Iterator<E> iterator() 
    {
        return stackList.iterator();
    }
    
}
