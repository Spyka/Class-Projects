//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
///     CS 310
///     Programming Assignment #4 - BalancedTree.java
///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

package data_structures;

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.TreeMap;


public class BalancedTree<K,V> implements DictionaryADT<K,V> {

        TreeMap<K,V> balancedTree;

        //CONSTRUCTOR
        public BalancedTree()
        {
                balancedTree = new TreeMap<K,V>();
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
        return balancedTree.containsKey(key);
}

// Adds the given key/value pair to the dictionary.  Returns
// false if the dictionary is full, or if the key is a duplicate.
// Returns true if addition succeeded.
public boolean insert(K key, V value)
{
        V v;

        if (key == null)
                return false;

        v = balancedTree.put(key,value);

        if (v != null)
                return false;

        return true;
}

// Deletes the key/value pair identified by the key parameter.
// Returns true if the key/value pair was found and removed,
// otherwise false.
public boolean remove(K key)
{
        V v = balancedTree.remove(key);

        if (v != null)
                return true;

        return false;
}

// Returns the value associated with the parameter key.  Returns
// null if the key is not found or the dictionary is empty.
public V getValue(K key)
{
        V value = balancedTree.get(key);
        return value;
}

// Returns the key associated with the parameter value.  Returns
// null if the value is not found in the dictionary.  If more
// than one key exists that matches the given value, returns the
// first one found.
public K getKey(V value)
{
        Iterator<V> valueIterator = values();
        Iterator<K> keyIterator = keys();

        V v;
        K k;

        if (isEmpty())
                return null;

        for (int i=0; i<(size() - 1); i++)
        {
                v = valueIterator.next();
                k = keyIterator.next();


                if (((Comparable<V>)value).compareTo(v) == 0)
                        return k;
        }

        return null;
}

// Returns the number of key/value pairs currently stored
// in the dictionary
public int size()
{
        return balancedTree.size();
}

// Returns true if the dictionary is at max capacity
public boolean isFull()
{
        return false;
}

// Returns true if the dictionary is empty
public boolean isEmpty()
{
        int size = balancedTree.size();

        if (size == 0)
                return true;

        return false;
}

// Returns the Dictionary object to an empty state.
public void clear()
{
        balancedTree.clear();

}

// Returns an Iterator of the keys in the dictionary, in ascending
// sorted order.  The iterator must be fail-fast.
public Iterator<K> keys()
{
        return balancedTree.keySet().iterator();
}

// Returns an Iterator of the values in the dictionary.  The
// order of the values must match the order of the keys.
// The iterator must be fail-fast.
public Iterator<V> values()
{
        return balancedTree.values().iterator();
}

}