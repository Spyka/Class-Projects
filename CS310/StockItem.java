//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
///     CS 310
///     Programming Assignment #4 - StockItem.java
///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

import java.util.Iterator;
import java.util.NoSuchElementException;
import data_structures.*;

public class StockItem implements Comparable<StockItem> {
    String SKU;
    String description;
    String vendor;
    float cost;
    float retail;

    // Constructor.  Creates a new StockItem instance.
    public StockItem(String SKU, String description, String vendor,
                     float cost, float retail)
    {
        this.SKU = SKU;
        this.description = description;
        this.vendor = vendor;
        this.cost = cost;
        this.retail = retail;
    }

    // Follows the specifications of the Comparable Interface.
    // The SKU is always used for comparisons, in dictionary order.
    public int compareTo(StockItem n)
    {
        int sku = Integer.parseInt(SKU);
        int compSku = Integer.parseInt(n.SKU);

        if (n == null)
        {
                System.out.println("Please insert a valid item to compare");
                throw new NoSuchElementException();
        }

        return sku - compSku;

    }

    // Returns an int representing the hashCode of the SKU.
    public int hashCode()
    {
        return Integer.parseInt(SKU);
    }

    // standard get methods
    public String getDescription()
    {
        return description;
    }

    public String getVendor()
    {
        return vendor;
    }

    public float getCost()
    {
        return cost;
    }

    public float getRetail()
    {
        return retail;
    }

    // All fields in one line, in order
    public String toString()
    {
        return SKU + ", " + description + ", " + vendor + ", "
                + cost + ", " + retail;
    }
}