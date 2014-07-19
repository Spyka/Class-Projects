//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///    
///    CS 310
///    Programming Assignment #1 - Queue.java
///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

package data_structures;

import java.util.Iterator;

public class Queue<E> implements Iterable<E>
{
    ListADT<E> queueList;

    public Queue()
    {
        queueList = new LinkedListDS<E>();
    }
    
// inserts the object obj into the queue
    public void enqueue(E obj)
    {
        queueList.addLast(obj);
    } 
    
    // removes and returns the object at the front of the queue  
    public E dequeue() 
    {
        //removeFirst()
        return queueList.removeFirst();
    }
    
    // returns the number of objects currently in the queue    
    public int size() 
    {
        //size()
        return queueList.size();
    }
    
    // returns true if the queue is empty, otherwise false  
    public boolean isEmpty() 
    {
        //isEmpty()
        return queueList.isEmpty();
    }
    
    // returns but does not remove the object at the front of the queue  
    public E peek() 
    {
        //peekFirst()
        return queueList.peekLast();
    }
    
    // returns true if the Object obj is in the queue    
    public boolean contains(E obj) 
    {
        //contains()
        return queueList.contains(obj);
    }
    
    // returns the queue to an empty state  
    public void makeEmpty() 
    {
        queueList.makeEmpty();
    }
    
    // removes the Object obj if it is in the queue and
    // returns true, otherwise returns false.
    public boolean remove(E obj)
    {
        //remove()
        return queueList.remove(obj);
    }
    
    // returns an iterator of the elements in the queue.  The elements
    // must be in the same sequence as dequeue would return them.    
    public Iterator<E> iterator() 
    {
      return queueList.iterator();
    }
    
}
