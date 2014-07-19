//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
///     CS 310
///     Programming Assignment #4 - ProductLookup.java
///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

import data_structures.*;
import java.util.Iterator;

public class ProductLookup
{
    private DictionaryADT<String,StockItem> dictionary;

    // Constructor.  There is no argument-less constructor, or default size
    public ProductLookup(int maxSize)
    {
        dictionary = new Hashtable<String,StockItem>(maxSize);
    }

    // Adds a new StockItem to the dictionary
    public void addItem(String SKU, StockItem item)
    {
        dictionary.insert(SKU,item);
    }

    // Returns the StockItem associated with the given SKU, if it is
    // in the ProductLookup, null if it is not.
    public StockItem getItem(String SKU)
    {
        return dictionary.getValue(SKU);
    }

    // Returns the retail price associated with the given SKU value.
    // -.01 if the item is not in the dictionary
    public float getRetail(String SKU)
    {
        StockItem stock = (StockItem) dictionary.getValue(SKU);

        if(stock == null)
                return -.01f;

        else
                return stock.getRetail();
    }

    // Returns the cost price associated with the given SKU value.
    // -.01 if the item is not in the dictionary
    public float getCost(String SKU)
    {
        StockItem stock = (StockItem) dictionary.getValue(SKU);

        if(!(dictionary.contains(SKU)))
                return -.01f;

        else
                return stock.getCost();
    }

    // Returns the description of the item, null if not in the dictionary.
    public String getDescription(String SKU)
    {
        StockItem stock = (StockItem) dictionary.getValue(SKU);

        if(!(dictionary.contains(SKU)))
                return null;

        else
                return stock.getDescription();
    }

    // Deletes the StockItem associated with the SKU if it is
    // in the ProductLookup.  Returns true if it was found and
    // deleted, otherwise false.
    public boolean deleteItem(String SKU)
    {
        return dictionary.remove(SKU);
    }

    // Prints a directory of all StockItems with their associated
    // price, in sorted order (ordered by SKU).
    public void printAll()
    {
        StockItem itemInQuestion;
        Iterator stockItems = values();

        while (stockItems.hasNext())
        {
                itemInQuestion = (StockItem) stockItems.next();
                System.out.println(itemInQuestion.getDescription() + "   " + itemInQuestion.getCost());
        }
    }

    // Prints a directory of all StockItems from the given vendor,
    // in sorted order (ordered by SKU).
    public void print(String vendor)
    {
        StockItem itemInQuestion;
        Iterator stockItems = values();

        while (stockItems.hasNext())
        {
                itemInQuestion = (StockItem) stockItems.next();

                if (((Comparable<String>)vendor).compareTo((String)itemInQuestion.getVendor()) == 0)
                        System.out.println(itemInQuestion.getDescription());
        }
    }

    // An iterator of the SKU keys.
    public Iterator<String> keys()
    {
        return dictionary.keys();
    }

    // An iterator of the StockItem values.
    public Iterator<StockItem> values()
    {
        return dictionary.values();
    }
}