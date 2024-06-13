/**
 * Assignment 1 - The Wacky Store
 *
 * Please read the comments below carefully, they describe your task in detail.
 *
 * There are also additional notes and clarifications on Quercus.
 *
 * Any clarifications and corrections will be posted to Piazza so please keep an
 * eye on the forum!
 *
 * Unauthorized distribution or posting is strictly prohibited. You must seek
 * approval from course staff before uploading and sharing with others.
 */

// No additional imports are allowed. You can make helper functions if you wish.
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 1024

typedef struct ItemNode ItemNode;
struct ItemNode {
    char name[MAX_NAME_LENGTH];
    int count;
    ItemNode* next;
};
/*
Note we realize that we have 2 notes here at least at the start 
Item node which is its own linked list 
And Customer is not a linked list it is just its on type 

The way these link is that customer has the pointer to the head of cart
*/
typedef struct Customer Customer;
struct Customer {
    char name[MAX_NAME_LENGTH];
    ItemNode* cart;
};


//this checkoutlane node is two dimensional, it can go forward or back
typedef struct CheckoutLaneNode CheckoutLaneNode;
struct CheckoutLaneNode {
    Customer* customer;

    CheckoutLaneNode* front;
    CheckoutLaneNode* back;
};

typedef struct CheckoutLane CheckoutLane;
struct CheckoutLane {
    CheckoutLaneNode* first;
    CheckoutLaneNode* last;
};

/**
 * Function: new_item_node
 * -----------------------
 * Allocate a new ItemNode. Allocation must be done manually (with malloc or
 * calloc). Initialize all variables using the arguments provided. Assume that
 * count will always be greater than 0.
 */
ItemNode* new_item_node(char* name, int count) {
    //first initialize new node
    ItemNode *new_item= NULL;
    new_item = (ItemNode *)malloc(sizeof(ItemNode));
    //next sanity check 
    if (new_item == NULL){
        printf("Error in allocating memory for new item\n");
        return NULL; //change this to something proper 
    }
    //adding values to this new node 
    strcpy(new_item->name, name);
    new_item->count = count;
    new_item->next = NULL;

    return new_item;
}

/**
 * Function: new_customer
 * ----------------------
 * Allocate a new Customer. Allocation must be done manually (with malloc or
 * calloc). Initialize all variables using the arguments provided.
 */
Customer* new_customer(char* name) {
    //first initialize new node 
    Customer* person = NULL;
    person = (Customer *)malloc(sizeof(Customer));
    //Sanity check 
    if (person == NULL){
        printf("Error in allocating memory for customer \n");
        return NULL;
    }
    //insert values into customer now 
    strcpy(person->name, name);
    person->cart = NULL;
    return person;
}

/**
 * Function: free_customer
 * -----------------------
 * Release all memory associated with a Customer back to the system. This
 * includes any items they may have had in their cart.
 */
void free_customer(Customer* customer) {
    //making sure that customer is not null sanity check
    if (customer != NULL){
        //initilizing helper pointer 
        ItemNode *pr = NULL;
        pr = customer->cart;
        //instant there is nothing in the cart
        if (pr == NULL){
            free(customer);
            customer = NULL;
        }
        else{

        while (pr != NULL){
            //creating a traversal node that point to what pr is pointing to
            ItemNode *t = pr->next;
            //freeing the values in pr
            free(pr);
            pr = NULL;
            //the we assign pr to the next value
            pr = t;
        }
        //then we just have to free customer 
        free(customer);
        customer = NULL;
        }
    }
}

/**
 * Function: open_new_checkout_line
 * --------------------------------
 * Allocate a new empty checkout lane. Allocation must be done manually (with
 * malloc or calloc).
 */
CheckoutLane* open_new_checkout_line() {
    // I am not over confident with this answer so REVISE THIS LATER 
    //i am first just setting up a new variables and allocating memory
    CheckoutLane *newlane = NULL;
    newlane = (CheckoutLane *)malloc(sizeof(CheckoutLane));
    //sanity check
    if (newlane == NULL){
        printf("error in allocating memory for open_new_checkout_line()\n");
        return NULL;
    }
    //initializing values 
    newlane->first = NULL;
    newlane->last = NULL;

    return newlane;
}

/**
 * Function: new_checkout_node
 * ---------------------------
 * Allocate a new CheckoutLaneNode. Allocation must be done manually (with
 * malloc or calloc). Initialize all variables using the arguments provided. Do
 * not allocate a new customer; instead copy the existing reference over.
 */
CheckoutLaneNode* new_checkout_node(Customer* customer) {
    // TODO: Complete this function.
    //just setting up variables and allocating memory
    CheckoutLaneNode *new_customer = NULL;
    new_customer = (CheckoutLaneNode *)malloc(sizeof(CheckoutLaneNode));
    //sanity check 
    if (new_customer == NULL){
        printf("error in allocating memory for new_checkout_node\n");
        return NULL;
    }
    //initializing values 
    new_customer->customer = customer;
    new_customer->front = NULL;
    new_customer->back = NULL;
    return new_customer;
}

/**
 * Function: add_item_to_cart
 * --------------------------
 * Add an item to a customer's cart, given its name and amount.
 *
 * If the given amount is 0 or less, do nothing.
 *
 * IMPORTANT: The items in a customer's cart should always be arranged in
 * lexicographically smallest order based on the item names. Consider the use of
 * the ASCII strcmp() function from <string.h>.
 *
 * No two ItemNodes in a customer's cart can have the same name.
 * If the customer already has an ItemNode with the same item name in their
 * cart, increase the node's count by the given amount instead.
 */

void adding_in_cart(Customer* customer, char* item_name, int amount){

    ItemNode *tr = customer->cart;
    ItemNode *pr = NULL;
    //THIS PART HERE IS GOING TO CHECK IF IT IS IN THE BODY 
    //initializing new_item 
    ItemNode *new_item = new_item_node(item_name, amount);
    while (tr != NULL){
        //finding the positiong where the item in cart has HIGHER lexigraphical then item
        if (strcmp(tr->name, item_name) == 0){
                tr->count = (tr->count) + amount;
                free(new_item);
                return;
            }

        else if (strcmp(tr->name, item_name) > 0){
            //This is checking if its at the head
            if (pr == NULL){
                new_item->next = tr;
                customer->cart = new_item;
                return;
            }
            //this will be if its not head
            new_item->next = tr;
            pr->next = new_item;
            return;
        }
        pr = tr;
        tr = tr->next;
    }

    //this means item is rather at head or tail
    //this one is inserting at head
    if (tr == NULL && pr == NULL){
        customer->cart = new_item;
        return;
    }

    //this means that the item is last in lexiographic order thus we must insert at tail 
    pr->next = new_item;
    return;
}

void add_item_to_cart(Customer* customer, char* item_name, int amount) {
    //sanity check
    if (amount > 0 && customer != NULL){
        //helper function use see above
        adding_in_cart(customer, item_name, amount);
        return;
    }
}

/**
 * Function: remove_item_from_cart
 * -------------------------------
 * Attempt to reduce the quantity of an item in a customer's cart, given its
 * name and amount.
 *
 * If no ItemNode in the customer's cart match the given item name, or the
 * amount given is <= 0, do nothing.
 *
 * If the quantity is reduced to a value less than or equal to 0, remove the
 * ItemNode from the customer's cart. This means you will need to do memory
 * cleanup as well.
 */
 int min(int amount, int amount2){
    int helper = amount - amount2;
    if (helper < 0){
        return 0;
    }
    return helper;
 }

void deleting_item_node(Customer* customer, ItemNode* tr, ItemNode* pr){
    //checking if the values is at the head
    if (pr == NULL){
        //we are moving the head to the pointer from the old head
        customer->cart = tr->next;
        free(tr);
        tr = NULL;
        return;
    }
    //checking if values is at the tail or in the body
    //all this does is takes the one before it and make it point to the one after the one we delete
    if (pr->next == NULL){
        pr->next = NULL;
        free(tr);
        tr = NULL;
        return;
    }

    pr->next = tr->next;
    free(tr);
    tr = NULL;
    return;
}

void remove_item_from_cart(Customer* customer, char* item_name, int amount) {
    //sanity check
    if (amount > 0 && customer != NULL){
        ItemNode *tr = customer->cart;
        ItemNode *pr = NULL;
        //locating item
        while (tr != NULL){
            if (strcmp(tr->name, item_name) == 0){
                //setting tr count to new values
                tr->count = min(tr->count, amount);
                //seeing if i need to remove item from cart
                if (tr->count == 0){
                    //deletes node if its at zero
                    deleting_item_node(customer, tr, pr);
                }
                return;
            }
            pr = tr;
            tr = tr->next;
        }
        //if its here it means that it did not find it in the cart
        return;
    }
}


/**
 * Function: total_number_of_items
 * -------------------------------
 * Count the total number of items in a customer's cart by summing all ItemNodes
 * and their associated quantities.
 */
int total_number_of_items(Customer* customer) {
    //sanity check
    if (customer != NULL){
        //initializing traverser
        ItemNode *p = customer->cart;
        int quantity = 0;
        //looping through customer
        while (p!=NULL){
            quantity = p->count + quantity;
            p = p->next;
        }
        return quantity;
    }
    return 0;
}

/**
 * Function: queue
 * ---------------
 * A customer has finished shopping and wishes to checkout. Add the given
 * customer to the end of the given checkout lane.
 */
void queue(Customer* customer, CheckoutLane* lane) {
    // TODO: Complete this function.
    //so what do we have as an input we have a checkoutlane so
    //we need to just loop through and add him to that lane

    //sanity check to see if they both have values 
    if (customer != NULL && lane != NULL){
        //initializing a new position
        CheckoutLaneNode *new_customer = new_checkout_node(customer);

        CheckoutLaneNode *tr = lane->first;
        //checking to see if there is a line 
        if (tr == NULL){
            lane->first = new_customer;
            lane->last = new_customer;
            return;
        }
        //now we know there is a line 
        tr = lane->last;
        //if this hold then only one person in the line
        if (tr == NULL){
            lane->first->back= new_customer;
            new_customer->front = lane->first;
            lane->last = new_customer;
            return;
        }
        //if we are here then there is more then 2 people in the line 
        new_customer->front = lane->last;
        lane->last->back = new_customer;
        lane->last = new_customer;
        return;
    }   
}

/**
 * Function: process
 * -----------------
 * Serve the customer at the head of the checkout lane and return the
 * total_number_of_items() the customer had in their cart.
 *
 * The customer leaves the store after being processed. Therefore, you must also
 * free any memory associated with them.
 *
 * If this function is called on an empty lane, return 0.
 */
int number_of_people(CheckoutLaneNode *line){
    int count = 0;
    while (line != NULL){
        count += 1;
        line = line->back;
    }
    return count;
}

int process(CheckoutLane* lane) {
    // TODO: Complete this function.
    //sanity check
    if (lane != NULL){
        CheckoutLaneNode *cr = lane->first;
        //sanity check
        if (cr == NULL){
            return 0;
        }
        Customer * customer = cr->customer;
        //sanity check
        if (customer == NULL){
            return 0;
        }
        //saving total number of items
        int number_of_items = total_number_of_items(customer);
        int num_peeps = number_of_people(cr);
        //now i have to move people in the queue accoringly 
        if (num_peeps == 1){
            //what do we know the back and the front are the same person 
            lane->first = NULL;
            lane->last = NULL;
        }
        if (num_peeps >= 2){
            lane->first = cr->back;
            lane->first->front = NULL;
            cr->back = NULL;
        }
        ///freeing customer
        free(cr);
        cr = NULL;
        free_customer(customer);
        return number_of_items;
    }
    return 0;
}

/**
 * Function: balance_lanes
 * -----------------------
 * Move a single customer from the end of the most busy checkout lane to the end
 * of the least busy checkout lane.
 *
 * Busyness is defined as the total number of customers in a checkout lane.
 *
 * If multiple lanes have the same busyness, select the lane that comes first in
 * the CheckoutLane* array.
 *
 * If the difference between the MAX and MIN checkout lanes is <= 1, do nothing.
 *
 * If there are less than 2 lanes, do nothing.
 *
 * Return true if and only if a customer was moved; otherwise false.
 */
 int initializing_lane_sort(CheckoutLane *lane){
    CheckoutLaneNode *cr = lane->first;
    int lowest_int = 0;
    while (cr != NULL){
        lowest_int +=1;
        cr = cr->back;
    }
    return lowest_int;
 }
 void lane_sort(CheckoutLane *lanes[], int number_of_lanes, CheckoutLane *order[], int sort[]){
    //im going to updating a list with the highest and lowest lanes 
    CheckoutLane *highest = NULL;
    CheckoutLane *lowest = NULL;
    int highest_int = 0;
    //this is me trying to intialize a starting value for lowest possible number of people in a lane 
    CheckoutLane *lane = *(lanes);
    if (lane == NULL){
        return;
    }
    *(order + 1) = lane;
    int lowest_int = initializing_lane_sort(lane);
    int lowest_check = lowest_int;
    // i need a way to initialize the lowest_int cause 0 will not work 
    //this is me looping through lanes[]
    for (int i = 0; i < number_of_lanes; i++){
       lane = *(lanes + i);
        //sanity check
        if (lane == NULL){
            return;
        }
        //this is me trying to find the count of people in this line
        CheckoutLaneNode *cr = lane->first;
        int count = 0;
        while (cr != NULL){
            count +=1;
            cr = cr->back;
        }
        //me checking if it its the highest number of people
        if (count > highest_int){
            highest_int = count;
            highest = lane;
        }
        //me checking if this is the lowest number of people
        if (count < lowest_int){
            lowest_int = count;
            lowest = lane;
        }
    }
    *(order) = highest;
    if (lowest_check != lowest_int){
        *(order + 1) = lowest;
    }
    sort[0] = highest_int;
    sort[1] = lowest_int;
    return;
 }
bool balance_lanes(CheckoutLane* lanes[], int number_of_lanes) {
    //these two thing will give me a list back that has [highest, lowest], as well as the [int #, int #]
    CheckoutLane *order[2] = {NULL, NULL};
    int sort[2];
    lane_sort(lanes, number_of_lanes, order, sort);
    CheckoutLane *highest = *(order);
    CheckoutLane *lowest = *(order +1);

    if (highest != NULL && lowest != NULL){
        if (sort[0]-sort[1]>1 || sort[1] - sort[0] < -1){
            //now i have to kind move one person from one lane to the lane somewhere else
            //is there only one person in the lane that is recieving is there no people etc 
            //queue(Customer* customer, CheckoutLane* lane) 
            CheckoutLaneNode *moving_person = highest->last;
            //i know that the lane that has this person currently has at least 2 people 
            int people = number_of_people((highest->first));
            if (people <= 2){
                //then the node first and last become one person 
                highest->last = highest->first;
            }
            queue(moving_person->customer, lowest);
            moving_person->front->back = NULL;
            moving_person->front = NULL;
            free(moving_person);
            moving_person = NULL;
            return true;
        }
    }
    return false;
}

/**
 * Function: process_all_lanes
 * ---------------------------
 * Given an array of CheckoutLane*, process() each CheckoutLane a single time
 * and return the the sum of the result.
 */
int process_all_lanes(CheckoutLane* lanes[], int number_of_lanes) {
    CheckoutLane *current_lane = NULL;
    int sum_of_items = 0;
    for (int i = 0; i<number_of_lanes; i++){
        current_lane = *(lanes + i);
        if (current_lane == NULL){
            return 0;
        }
        CheckoutLaneNode *cr = current_lane->first;
        int count = 0;
        while (cr != NULL){
           cr = cr->back;
           count += 1;
        }
        for (int j = 0; j< count; j++){
            sum_of_items = sum_of_items + process(current_lane);
        }
    }
    return sum_of_items;
}

/**
 * Function: close_store
 * ---------------------
 * It's closing time. Given an array of CheckoutLane*, free all memory
 * associated with them. Any customers still left in the queue is kicked out and
 * also freed from memory.
 */
void close_store(CheckoutLane* lanes[], int number_of_lanes) {
    //this clear all the memory from each lane
    process_all_lanes(lanes, number_of_lanes);
    //then i need  to clear all the lanes itself
    CheckoutLane *pr = NULL;
    for (int i = 0; i < number_of_lanes; i++){
        pr = *(lanes + i);
        free(pr);
        pr = NULL;
    }
    return;
}
