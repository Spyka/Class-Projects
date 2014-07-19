//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
///     CS 310
///     Programming Assignment #1 - LinkedListDS.java
///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

package data_structures;

import java.util.Iterator;

import java.util.NoSuchElementException;

public class LinkedListDS<E> implements ListADT<E>
{

        //Class Variables - head and tail pointer + tracker for list size
        private Node<E> head, tail;
        private int currentSize;


        //Constructor
        public LinkedListDS ()
        {
                head = tail = null;
                currentSize = 0;
        }

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
///     METHODS
///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

        //  Adds the Object obj to the beginning of the list (Big O of 1)
        public void addFirst(E obj)
        {
                Node<E> newNode = new Node<E>(obj);
                if (head == null)
                        head = tail = newNode;
                else
                {
                        newNode.next = head;    //head becomes the next node
                        head = newNode;         //head now points to the new node
                }

                currentSize++;
        }

        //  Adds the Object obj to the end of the list (Big O of 1)
        public void addLast(E o)
        {
                Node<E> newNode = new Node<E>(o);
                if (head == null)
                        head = tail = newNode;
                else
                {
                        tail.next = newNode;    //new node becomes the next node
                        tail = newNode;         //tail now points to the new node
                }

                currentSize++;
        }

        //  Removes the first Object in the list and returns it. (Big O of 1)
        //  Returns null if the list is empty.
        public E removeFirst()
        {
                Node<E> currentNode = head;

                if (head == null)                       //if the list is empty
                        return null;

                if (head == tail)
                {
                        head = tail = null;             //Accounting for only
                        currentSize--;                  //one object in the list
                        return currentNode.data;
                }

                if (currentNode.next != null)
                        head = currentNode.next;        //If more than one object
                                                        //in the list
                currentSize--;
                return currentNode.data;
        }

        //  Removes the last Object in the list and returns it. (Big O of n)
        //  Returns null if the list is empty.
        public E removeLast()
        {
                Node<E> previousNode = null, current = head;

                while (current != tail)                 //traversing the list
                {
                        previousNode = current;
                        current = current.next;
                }

                if (head == null)                       //if the list is empty
                        return null;
                if (head == tail)                       //if there is only one
                        head = tail = null;             //object in the list
                else
                {
                        previousNode.next = null;       //removing the tail node
                        tail = previousNode;            //and reassigning the tail
                }

                currentSize--;
                return current.data;

        }

        //  Returns the first Object in the list, but does not remove it.
        //  Returns null if the list is empty.
        public E peekFirst()
        {
                if(head != null)
                        return head.data;
                else
                        return null;
        }

        //  Returns the last Object in the list, but does not remove it.
        //  Returns null if the list is empty.
        public E peekLast()
        {
                if (tail != null)
                        return tail.data;
                else
                        return null;
        }

        //  Finds and returns the Object obj if it is in the list, otherwise
        //  returns null.  Does not modify the list in any way
        public E find(E obj)
        {

                Node<E> currentNode = head, previousNode = null;

                if (head == null)               //check for empty
                        return null;

                while (currentNode != null &&
                        ((Comparable<E>)obj).compareTo(currentNode.data) != 0)
                {
                        previousNode = currentNode;             //traverse the list
                        currentNode = currentNode.next;         //if not null and the
                }                                               //object has not been
                                                                //found
                if (currentNode == null)        //went through
                        return null;            //the whole list and found nothing

                if (((Comparable<E>)obj).compareTo(currentNode.data) == 0)
                {
                        if (currentNode == head)                //object is the head
                                head = currentNode.next;

                        else if (currentNode == tail)           //object is the tail
                        {
                                previousNode.next = null;
                                tail = previousNode;
                        }

                        else                                    //object is in the list
                                previousNode.next = currentNode.next;

                        if (head == null)               //the object was the only thing in
                                tail = null;            //the list
                }

                        return currentNode.data;

        }

        //  Removes the first instance of thespecific Object obj from the list, if it exists.
        //  Returns true if the Object obj was found and removed, otherwise false
        public boolean remove(E obj)
        {
                Node<E> currentNode = head, previousNode = null;

                if (head == null)
                        return false;

                while (currentNode != null &&
                        ((Comparable<E>)obj).compareTo(currentNode.data) != 0)
                {
                        previousNode = currentNode;             //traverse the list if not
                        currentNode = currentNode.next;         //null and the object has
                }                                               //not been found

                if (currentNode == null)        //went through the whole list and
                        return false;           //found nothing

                if (((Comparable<E>)obj).compareTo(currentNode.data) == 0)
                {
                        if (currentNode == head)                //object is the head
                                head = currentNode.next;

                        else if (currentNode == tail)           //object is the tail
                        {
                                previousNode.next = null;
                                tail = previousNode;
                        }

                        else                                    //object is in the list
                                previousNode.next = currentNode.next;

                        if (head == null)               //the object was the only thing
                                tail = null;            //in the list
                }

                        currentSize--;
                        return true;
        }

        //  The list is returned to an empty state.
        public void makeEmpty()
        {
                head = tail = null;
                currentSize = 0;
        }

        //  Returns true if the list contains the Object obj, otherwise false
        public boolean contains(E obj)
        {
                Node<E> currentNode = head;

                while (currentNode != null &&
                        ((Comparable<E>)obj).compareTo(currentNode.data) != 0)

                        currentNode = currentNode.next;         //traverse the list if not
                                                                //null and object not found
                if (currentNode == null)
                        return false;           //object not in list

                return true;                    //object in list
        }

        //  Returns true if the list is empty, otherwise false
        public boolean isEmpty()
        {
                if (head == tail && tail == null)
                        return true;
                else
                        return false;
        }

        //  Returns true if the list is full, otherwise false
        public boolean isFull()
        {
                return false;
        }

        //  Returns the number of Objects currently in the list.
        public int size()
        {
                return currentSize;
        }

        //  Returns an Iterator of the values in the list, presented in
        //  the same order as the list.
        public Iterator<E> iterator()
        {
                return new IteratorHelper();
        }

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
///
///     CLASSES
///
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

        class Node<T>
        {
                T data;
                Node<T> next;

                //Constructor
                public Node(T obj)
                {
                        data = obj;
                        next = null;
                }
        }

        public class IteratorHelper implements Iterator<E>
        {
                Node<E> iterPtr;

                //Constructor
                public IteratorHelper()
                {
                        iterPtr = head;
                }
                        public boolean hasNext()
                        {
                                return iterPtr != null;
                        }

                        public E next()
                        {
                                if (!hasNext())
                                        throw new NoSuchElementException();
                                E tmp = iterPtr.data;
                                iterPtr = iterPtr.next;
                                return tmp;
                        }

                        public void remove()
                        {
                                throw new UnsupportedOperationException();
                        }

        }

}