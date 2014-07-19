//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
///     CS 310
///     Programming Assignment #4 - BinarySearchTree.java
///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

package data_structures;

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.ConcurrentModificationException;


public class BinarySearchTree<K,V> implements DictionaryADT<K,V> {

        private int currentSize;
        private int modCounter;
        private Wrapper<K,V> rootNode;
        private K foundKey;

        //CONSTRUCTOR
        public void BinarySearchTree()
        {
                modCounter = 0;
                currentSize = 0;
                rootNode = null;
        }

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
////////////                                    //////////////
////////////            METHODS                 //////////////
////////////                                    //////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

// Returns true if the dictionary has an object identified by
// key in it, otherwise false.
public boolean contains(K key)
{
        Wrapper<K,V> currentNode = rootNode;

        if (isEmpty())
                return false;

        //while the currentNode's key is not equal to the key in question
        //traverse the tree, checking to see whether to move right or left
        //based off of greater than or less than
        while (((Comparable<K>)currentNode.key).compareTo((K)key) != 0)
        {
                if (((Comparable<K>)key).compareTo((K)currentNode.key) < 0)
                        currentNode = currentNode.leftChild;
                else
                        currentNode = currentNode.rightChild;

                //if hit the end of the tree
                if (currentNode == null)
                        return false;
        }

        //if find the key
        return true;
}

// Adds the given key/value pair to the dictionary.  Returns
// false if the dictionary is full, or if the key is a duplicate.
// Returns true if addition succeeded.
public boolean insert(K key, V value)
{
        Wrapper<K,V> parentNode, currentNode = rootNode;
        Wrapper<K,V> newNode = new Wrapper<K,V>(key,value);

        if (isEmpty())
        {
                rootNode = newNode;
                currentSize++;
                modCounter++;
                return true;
        }

        //for all compare the keys
        for(;;)
        {
                //if the tree already contains the key
                if (((Comparable<K>)key).compareTo((K)currentNode.key) == 0)
                        return false;

                //traverse left until find correct spot
                if (((Comparable<K>)key).compareTo((K)currentNode.key) < 0)
                {
                        if (currentNode.leftChild == null)
                        {
                                currentNode.leftChild = newNode;
                                currentSize++;
                                modCounter++;
                                return true;
                        }

                        else
                                currentNode = currentNode.leftChild;
                }

                //traverse right until find correct spot
                else if (((Comparable<K>)key).compareTo((K)currentNode.key) > 0)
                {
                        if (currentNode.rightChild == null)
                        {
                                currentNode.rightChild = newNode;
                                currentSize++;
                                modCounter++;
                                return true;
                        }

                        else
                                currentNode = currentNode.rightChild;
                }
        }
}

// Deletes the key/value pair identified by the key parameter.
// Returns true if the key/value pair was found and removed,
// otherwise false.
public boolean remove(K key)
{
        Wrapper<K,V> parentNode = null, currentNode = rootNode;
        boolean vc = false;

        if (isEmpty())
                return false;

        //while the key is not found traverse the tree
        while (((Comparable<K>)key).compareTo((K)currentNode.key) != 0)
        {
                parentNode = currentNode;

                //decide whether to move left or right down the tree
                if (((Comparable<K>)key).compareTo((K)currentNode.key) > 0)
                {       currentNode = currentNode.rightChild;
//                      System.out.println("first while greater than 0");
                }
                else
                {       currentNode = currentNode.leftChild;
//                      System.out.println("first while less than 0");
                }
        }

        //if found the key, look at the different cases invoving children
        if (((Comparable<K>)key).compareTo((K)currentNode.key) == 0)
                vc = variousCases(currentNode, parentNode);

        if (vc == true)
        {
//              System.out.println("vc true");
                currentSize--;
                modCounter++;
                return vc;
        }


        return false;
}

public boolean variousCases(Wrapper<K,V> currentNode, Wrapper<K,V> parentNode)
{
        boolean tc;
        Wrapper<K,V> sucessorNode, shiftNode;

        //If has no children
        if (currentNode.leftChild == null && currentNode.rightChild == null)
        {
//              System.out.println("vc if no children");
                if (parentNode == null)
                        rootNode = null;

                else if (parentNode.leftChild == currentNode)
                        parentNode.leftChild = null;

                else if (parentNode.rightChild == currentNode)
                        parentNode.rightChild = null;

                return true;
        }

        //If has a left child
        else if (currentNode.leftChild != null && currentNode.rightChild == null)
        {
//              System.out.println("vc if left child");
                if (parentNode == null)
                        rootNode = currentNode.leftChild;

                else if (parentNode.leftChild == currentNode)
                        parentNode.leftChild = currentNode.leftChild;

                else if (parentNode.rightChild == currentNode)
                        parentNode.rightChild = currentNode.leftChild;

                return true;
        }

        //If has a right child
        else if (currentNode.leftChild == null && currentNode.rightChild != null)
        {
//              System.out.println("vc if right child");
                if (parentNode == null)
                        rootNode = currentNode.rightChild;

                else if (parentNode.leftChild  == currentNode)
                        parentNode.leftChild = currentNode.rightChild;

                else if (parentNode.rightChild == currentNode)
                        parentNode.rightChild = currentNode.rightChild;

                return true;
        }

        //If has two children
        else if (currentNode.leftChild != null && currentNode.rightChild != null)
        {
//              System.out.println("vc if two children");
                //Go Right
                shiftNode = currentNode.rightChild;
                sucessorNode = shiftNode;

                //After going right, go left as far as possible
                while (shiftNode.leftChild != null)
                {
                        sucessorNode = shiftNode;
                        shiftNode = shiftNode.leftChild;
                }

//              System.out.println("while completed");

                if (parentNode == null)
                {
//                      System.out.println("two children parent null");
                        rootNode.key = shiftNode.key;
                        rootNode.value = shiftNode.value;

                        //control for floating children
                        tc = treeCleanUp(shiftNode, rootNode, sucessorNode);

                        if (tc == true)
                        {
//                              System.out.println("tc parent null true");
                                return true;
                        }

                        else
                        {
//                              System.out.println("tc parent null false");
                                return false;
                        }
                }

                //The final left node has one child
                else
                {
//                      System.out.println("two children parent not null");
                        currentNode.key = shiftNode.key;
                        currentNode.value = shiftNode.value;

                        //control for floating childre
                        tc = treeCleanUp(shiftNode, currentNode, sucessorNode);

                        if (tc == true)
                        {
//                              System.out.println("tc parent  not null true");
                                return true;
                        }

                        else
                        {
//                              System.out.println("tc parent not null false");
                                return false;
                        }
                }

        }

        return false;
}

public boolean treeCleanUp(Wrapper<K,V> shiftNode, Wrapper<K,V> currentNode,
                                Wrapper<K,V> sucessorNode)
{
//      System.out.println("tree cleanup");
        //Relinks floating children


        if (shiftNode.rightChild != null)
        {
                if (currentNode.rightChild == shiftNode)
                        currentNode.rightChild = shiftNode.rightChild;

                else
                        sucessorNode.leftChild = shiftNode.rightChild;

                return true;
        }

        else if (shiftNode.rightChild == null)
        {
                if (currentNode.rightChild == shiftNode)
                        currentNode.rightChild = null;

                else if (sucessorNode.leftChild == shiftNode)
                        sucessorNode.leftChild = null;

                return true;
        }


        return false;
}

// Returns the value associated with the parameter key.  Returns
// null if the key is not found or the dictionary is empty.
public V getValue(K key)
{
        Wrapper<K,V> currentNode = rootNode;

        if (rootNode == null)
                return null;

        //search the tree for the correct key
        while (((Comparable<K>)currentNode.key).compareTo((K)key) != 0)
        {
                //moving right or left
                if (((Comparable<K>)key).compareTo((K)currentNode.key) < 0)
                        currentNode = currentNode.leftChild;
                else
                        currentNode = currentNode.rightChild;

                //if the entire tree is traversed and the key isn't there
                if (currentNode == null)
                        return null;
        }

        //if the key is found
        return currentNode.value;
}

// Returns the key associated with the parameter value.  Returns
// null if the value is not found in the dictionary.  If more
// than one key exists that matches the given value, returns the
// first one found.
public K getKey(V value)
{
        valueFinder(rootNode,value);

        return foundKey;
}

public K valueFinder(Wrapper<K,V> n, V value)
{

        if (n == null)
                return null;

        //check the value in question against the value of the node
        //being looked at, if equal return the key
        if (((Comparable<V>)value).compareTo(n.value) == 0)
        {
                foundKey = n.key;
                return foundKey;
        }

        valueFinder(n.leftChild,value);
        valueFinder(n.rightChild,value);

        return null;
}

// Returns the number of key/value pairs currently stored
// in the dictionary
public int size()
{
        return currentSize;
}

// Returns true if the dictionary is at max capacity
public boolean isFull()
{
        return false;
}

// Returns true if the dictionary is empty
public boolean isEmpty()
{
        if (rootNode == null)
                return true;
        else
                return false;
}

// Returns the Dictionary object to an empty state.
public void clear()
{
        currentSize = 0;
        modCounter = 0;
        rootNode = null;
}

// Returns an Iterator of the keys in the dictionary, in ascending
// sorted order.  The iterator must be fail-fast.
public Iterator<K> keys()
{
        return new KeyIteratorHelper();
}

// Returns an Iterator of the values in the dictionary.  The
// order of the values must match the order of the keys.
// The iterator must be fail-fast.
public Iterator<V> values()
{
        return new ValueIteratorHelper();
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
////////////                                    //////////////
////////////            CLASSES                 //////////////
////////////                                    //////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

public class Wrapper<K,V> implements Comparable<Wrapper<K,V>>
{
        private K key;
        private V value;
        private Wrapper<K,V> leftChild;
        private Wrapper<K,V> rightChild;

        public Wrapper(K k, V v)
        {
                key = k;
                value = v;
                leftChild = rightChild = null;
        }

        public int compareTo(Wrapper<K,V> w)
        {
                return ((Comparable<K>)key).compareTo((K)w.key);
        }
}

class KeyIteratorHelper<K> extends IteratorHelper<K>
{
        public KeyIteratorHelper()
        {
                super();
        }

        public K next()
        {
                return (K) array[index++].key;
        }
}

class ValueIteratorHelper<V> extends IteratorHelper<V>
{
        public ValueIteratorHelper()
        {
                super();
        }

        public V next()
        {
                return (V) array[index++].value;
        }
}

abstract class IteratorHelper<E> implements Iterator<E>
{
        protected Wrapper<K,V>[] array;
        protected long modCheck;
        protected int index, idx;

        //Constructor
        public IteratorHelper()
        {
                array = new Wrapper[currentSize];
                modCheck = modCounter;
                idx = index = 0;
                inOrder(rootNode);
        }

        public boolean hasNext()
        {
                if (modCheck != modCounter)
                        throw new ConcurrentModificationException();

                if (index < currentSize)
                        return true;
                else
                        return false;
        }

        public abstract E next();

        private void inOrder (Wrapper<K,V> n)
        {
                if (n != null)
                {
                        inOrder(n.leftChild);
                        array[idx++] = n;
                        inOrder(n.rightChild);
                }

                return;
        }

        public void remove()
        {
                throw new UnsupportedOperationException();
        }
}

}