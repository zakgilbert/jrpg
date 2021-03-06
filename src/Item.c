
#include "Item.h"

static char *ITEMS[] = {
    FOREACH_ITEM(GENERATE_STRING)};

static void swap(int *array, int sw1, int sw2)
{
    int temp = array[sw1];
    array[sw1] = array[sw2];
    array[sw2] = temp;
}

static void _destroy(Item *this)
{
    if (NULL != this)
    {
        free(this);
        this = NULL;
    }
}
/*
 *  fill_bag
 *      After calling CREATE_BAG, fill the bag with the items you desire.
 *      The parameters items and quantities both correspond to num_items
 *      as follows.
 *  (Items *this)       --> the object
 *  (int * items)       --> the list of item keys(enum values).
 *  (int * quantities)  --> the list of each items quantity.
 *  (int num_items)     --> the amount of items being added.
 *   
 *  In the following case you would have 4 POTIONS, and 5 ETHERS put in the bag.
 *      int items[2] =       {POTION, ETHER}
 *      int quantities[2] =  {4,5}
 *      int num_items =      2;
 */
static void _fill_bag(Item *this, int *items, int *quantities, int len)
{
    int i;

    this->items = (int *)malloc(sizeof(int) * len);
    this->item_quantities = (int *)malloc(sizeof(int) * len);

    for (i = 0; i < len; i++)
    {
        this->items[i] = items[i];
        if (NULL == quantities)
            this->item_quantities[i] = 1;
        else
            this->item_quantities[i] = quantities[i];
    }
    this->items_in_bag = len;
}
static char *_add_item(Item *this, ITEM_ENUM item_enum)
{
    size_t new_size = this->items_in_bag + 1;

    this->items = realloc(this->items, sizeof(int) * new_size);
    this->item_quantities = realloc(this->item_quantities, sizeof(int) * new_size);

    this->items[this->items_in_bag] = item_enum;
    this->item_quantities[this->items_in_bag] = 1;
    this->items_in_bag++;
    return ITEMS[this->items[this->items_in_bag - 1]];
}

static int _remove_item(Item *this, int item_index)
{
    int last_item;
    last_item = this->items_in_bag - 1;
    while (item_index != last_item)
    {
        swap(this->items, item_index, item_index + 1);
        swap(this->item_quantities, item_index, item_index + 1);
        item_index++;
    }
    this->items = realloc(this->items, sizeof(int) * last_item);
    this->item_quantities = realloc(this->item_quantities, sizeof(int) * last_item);
    this->items_in_bag = last_item;

    return -1;
}
static int _decrement_item(Item *this, ITEM_ENUM item_enum)
{

    int item_index, item_was_removed;

    item_was_removed = 0;
    item_index = this->find_item(this, item_enum);
    if (item_index < 0)
    {
        printf("\nThe item was not decremented");
    }
    else
    {
        if (this->item_quantities[item_index] == 1)
        {
            this->item_quantities[item_index]--;
            this->remove_item(this, item_index);
            item_was_removed = -1;
        }
        else
        {
            this->item_quantities[item_index]--;
        }
    }
    return item_was_removed;
}

static int _find_item(Item *this, ITEM_ENUM item_enum)
{
    int i, item_index;
    item_index = -1;

    for (i = 0; i < this->items_in_bag; i++)
    {
        if (this->items[i] == item_enum)
            item_index = i;
    }
    return item_index;
}

static int _quaff_item(Item *this, Affect *affect)
{
    int item_was_quaffed, item_was_removed;

    item_was_quaffed = 0;
    item_was_removed = 0;
    this->affect = affect;

    if (((item_was_quaffed) = (this->affect->cause_affect(affect))))
        item_was_removed = this->decrement_item(this, this->affect->affect_enum);

    this->affect->destroy(this->affect);
    return item_was_removed;
}

/************************************************
 * Check if the item already exits in this bag.
 * If not call add_item.
 * Otherwise increment the existing item.
 * Return the name of the item added as a char *.
*/
static char *_loot(Item *this, ITEM_ENUM item_enum)
{
    int item_index;

    item_index = this->find_item(this, item_enum);

    if (item_index == -1)
        return this->add_item(this, item_enum);
    else
    {
        this->item_quantities[item_index]++;
        printf("\nYour \"%s\" has increased from %d to %d.", ITEMS[this->items[item_index]], this->item_quantities[item_index] - 1, this->item_quantities[item_index]);
        return ITEMS[this->items[item_index]];
    }
}

static void _update_quant_disp(Item *this)
{
    int i;
    char temp[10];
    if (NULL != this->display)
    {
        for (i = 0; i < this->items_in_bag; i++)
            free(this->display[i]);
        this->display = NULL;
    }
    this->display = (char **)malloc(sizeof(char *) * this->items_in_bag);
    for (i = 0; i < this->items_in_bag; i++)
    {
        sprintf(temp, "%d", this->item_quantities[i]);
        this->display[i] = (char *)malloc(strlen(temp));
        this->display[i] = strcpy(this->display[i], temp);
    }
}
static const char *_get_display(Item *this, int index)
{
    return this->display[index];
}

Item *CREATE_BAG()
{
    Item *this = (Item *)malloc(sizeof(*this));

    this->destroy = _destroy;
    this->fill_bag = _fill_bag;
    this->add_item = _add_item;
    this->find_item = _find_item;
    this->decrement_item = _decrement_item;
    this->quaff_item = _quaff_item;
    this->remove_item = _remove_item;
    this->loot = _loot;
    this->get_display = _get_display;
    this->update_quant_disp = _update_quant_disp;

    this->item_quantities = NULL;
    this->items = NULL;
    this->items_in_bag = 0;
    this->affect = NULL;
    this->display = NULL;

    return this;
}
