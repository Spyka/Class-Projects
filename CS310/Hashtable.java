//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
///     CS 310
///     Programming Assignment #4 - Hashtable.java
///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

package data_structures;

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.ConcurrentModificationException;


public class Hashtable<K,V> implements DictionaryADT<K,V>
{

        private int tableSize, maxSize, currentSize;
        private long modCounter;
        private ListADT<Wrapper<K,V>>[] list;


        //CONSTRUCTOR
        public Hashtable(int maxSize)
        {
                modCounter = 0;
                currentSize = 0;
                this.maxSize = maxSize;
                tableSize = maxSize + (maxSize/3);
                list = new LinkedListDS[tableSize];

                for (int i=0; i<tableSize; i++)
                        list[i] = new LinkedListDS<Wrapper<K,V>>();
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
        //l is the list to where the key hashes to
        //find the keyNode inside l
        ListADT<Wrapper<K,V>> l = (list[getIndex(key)]);
        Wrapper<K,V> keyNode = l.find(new Wrapper<K,V>(key,null));

        //if l does contain the keyNode
        if (keyNode != null)
                return true;

        //if l does not contain the keyNode
        return false;
}

// Adds the given key/value pair to the dictionary.  Returns
// false if the dictionary is full, or if the key is a duplicate.
// Returns true if addition succeeded.
public boolean insert(K key, V value)
{
        Wrapper<K,V> newNode = new Wrapper<K,V>(key, value);

        if (isFull())
                return false;

        //add the newNode to the end of the list where they key hashes to
        list[getIndex(key)].addLast(newNode);

        currentSize++;
        modCounter++;

        return true;
}

// Deletes the key/value pair identified by the key parameter.
// Returns true if the key/value pair was found and removed,
// otherwise false.
public boolean remove(K key)
{
/*      Wrapper<K,V> findNode = new Wrapper<K,V>(key,null);
//      LinkedListDS<Wrapper<K,V>> l = (list[getIndex(key)]);
        if (list[getIndex(key)].remove(findNode))
        {
                currentSize--;
                modCounter++;
                return true;
        }

        return false;
*/

/*      Wrapper<K,V> keyNode = l.find(new Wrapper<K,V>(key,null));

        if (keyNode == null)
                return false;

        else
        {
                return l.remove(keyNode);
        }
*/
        //l is the list where the key hashes to
        //remove the node in question from the list l
        ListADT<Wrapper<K,V>> l = (list[getIndex(key)]);
        if (l.remove(new Wrapper<K,V>(key,null)))
        {
                currentSize--;
                modCounter++;
                return true;
        }

        return false;
}

// Returns the value associated with the parameter key.  Returns
// null if the key is not found or the dictionary is empty.
public V getValue(K key)
{
        //l is the list that the key hashes to
        //find the keyNode in l
        ListADT<Wrapper<K,V>> l = (list[getIndex(key)]);
        Wrapper<K,V> keyNode = l.find(new Wrapper<K,V>(key,null));

        //if the keyNode is not in l
        if (l == null)
                return null;

        //if the keyNode is in l
        else
                return keyNode.value;

}

// Returns the key associated with the parameter value.  Returns
// null if the value is not found in the dictionary.  If more
// than one key exists that matches the given value, returns the
// first one found.
public K getKey(V value)
{
        Iterator<K> kIter = keys();
        Iterator<V> vIter = values();

        K tempK;
        V tempV;

        while (kIter.hasNext())
        {
                //get each key and value from the iterators
                tempK = kIter.next();
                tempV = vIter.next();

                //if the iterator value is equal to the value being
                //searched for return the key
                if (((Comparable<V>)tempV).compareTo(value) == 0)
                        return tempK;
        }

        return null;

/*      for (int i=0; i < tableSize; i++)
                for (Wrapper n : list[i])
                {
                        if (n == null)
                                System.out.println("n is null");

                        if (((Comparable<V>)value).compareTo((V)n.value) == 0)
                                return (K) n.key;

                        System.out.println("attempted comparable");
                }

        return null;
*/

/*      for (int i=0; i < tableSize; i++)
                for (Wrapper n : list[i])
                        nodes[j++] = n;

        if (nodes[idx].value == null)
                System.out.println("nodes[idx].value = null");

        while (nodes[idx].value != value)
                idx++;

        if (nodes[idx] == null)
                return null;

        return nodes[idx].key;
*/
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
        if (currentSize == maxSize)
                return true;
        else
                return false;
}

// Returns true if the dictionary is empty
public boolean isEmpty()
{
        if (currentSize == 0)
                return true;
        else
                return false;
}

// Returns the Dictionary object to an empty state.
public void clear()
{
        currentSize = 0;
        modCounter = 0;
        list = null;
}

// Returns an Iterator of the keys in the dictionary, in ascending
// sorted order.  The iterator must be fail-fast.
public Iterator<K> keys()
{
        return new KeyIteratorHelper<K>();
}

// Returns an Iterator of the values in the dictionary.  The
// order of the values must match the order of the keys.
// The iterator must be fail-fast.
public Iterator<V> values()
{
        return new ValueIteratorHelper<V>();
}

public int getIndex(K key)
{
        return (key.hashCode() & 0x7FFFFFFF) % tableSize;
}


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
////////////                                    //////////////
////////////            CLASSES                 //////////////
////////////                                    //////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

class Wrapper<K,V> implements Comparable<Wrapper<K,V>>
{
        K key;
        V value;

        public Wrapper(K k, V v)
        {
                this.key = k;
                this.value = v;
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
                return (K) nodes[idx++].key;
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
                return (V) nodes[idx++].value;
        }
}

abstract class IteratorHelper<E> implements Iterator<E>
{
        protected Wrapper<K,V>[] nodes;
        protected Wrapper<K,V>[] n;
        protected int idx, j;
        protected long modCheck;

        public IteratorHelper()
        {
                nodes = new Wrapper[currentSize];
                idx = 0;
                j = 0;
                modCheck = modCounter;

                for (int i=0; i < tableSize; i++)
                        for (Wrapper wrap : list[i])  //for every wrapper
                                nodes[j++] = wrap;    //in l place the node
                                                      //in the nodes array

                //sort the array partially and then completely
                nodes = (Wrapper<K,V>[]) shellSort(nodes);
                nodes = (Wrapper<K,V>[]) insertionSort(nodes);

//              traverseAndSort();
        }

/*      public void traverseAndSort()
        {
                for (int i=0; i < tableSize; i++)
                        for (Wrapper n : list[i])
                                nodes[j++] = n;

                nodes = (Wrapper<K,V>[]) shellSort(nodes);
                //nodes = (Wrapper<K,V>[]) insertionSort(nodes);
        }
*/
        public boolean hasNext()
        {
                if (modCheck != modCounter)
                        throw new ConcurrentModificationException();

                return idx < currentSize;
        }

        public abstract E next();

        public void remove()
        {
                throw new UnsupportedOperationException();
        }

        public Wrapper<K,V>[] shellSort(Wrapper<K,V>[] array)
        {
                n = array;
                Wrapper<K,V> temp;
                int in, out;
                int h=1;
                int size = n.length;

                while (h <= size/3)
                        h = h*3+1;

                while (h > 0)
                {
                        for (out=h; out<size; out++)
                        {
                                temp = n[out];
                                in = out;

                                if (n[in-h] == null)
                                        System.out.println("n[in-h] is null");

                                if (temp == null)
                                        System.out.println("temp is null");

                                while ((in > h-1) && (((Comparable<K>)
                                        n[in-h].key).compareTo(temp.key) >= 0 ))
                                {
                                        n[in] = n[in-h];
                                        in -= h;
                                }

                                n[in] = temp;
                        }

                        h = (h-1)/3;
                }

                return n;
        }

        public Wrapper<K,V>[] insertionSort(Wrapper<K,V> array[])
        {
                n = array;
                int in, out;
                Wrapper<K,V> temp;

                for (out = 1; out < n.length; out++)
                {
                        temp = n[out];
                        in = out;

                        while (in > 0 && (((Comparable<K>)
                                n[in-1].key).compareTo(temp.key) > 0))
                        {
                                n[in] = n[in-1];
                                in--;
                        }

                        n[in] = temp;
                }

                return n;
        }

}


}