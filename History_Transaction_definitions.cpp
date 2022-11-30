#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

// g++ -o main History_Transaction_definitions.cpp project4.cpp main.cpp -std=c++11
////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

/*PUBLIC MEMBER VARIABLES:
All the class members declared under public will be available to everyone.
The data members and member functions declared public can be accessed by
other classes too. The public members of a class can be accessed from
anywhere in the program using the direct member access operator (.) with
the object of that class.
*/

/*PRIVATE MEMBER VARIABLES:
The class members declared as private can be accessed only by the
functions inside the class.
They are not allowed to be accessed directly by any object or
function outside the class. Only the member functions or
the friend functions are allowed to access the private data members
of a class.
*/

// Constructor
// TASK 1
//
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date, unsigned int month_date,
                         unsigned year_date, bool buy_sell_trans, unsigned int number_shares, double trans_amount)
{
  symbol = ticker_symbol;      // declare std::string ticker_symbol variable
  day = day_date;              // declare day_date variable
  month = month_date;          // declare month_date variable
  year = year_date;            // declare year_date variable
  trans_type = buy_sell_trans; // declare buy_sell_trans variable
  shares = number_shares;      // declare number_shares variable
  amount = trans_amount;       // declare trans_amount variable
  trans_id = assigned_trans_id;
  assigned_trans_id++;
  p_next = nullptr;
  {
    if (buy_sell_trans)
    {
      trans_type = "Buy";
    }
    else
    {
      trans_type = "Sell";
    }
  }
  // Destructor
  // TASK 1
  //
}

Transaction::~Transaction()
{
  /*
  if (p_next != nullptr)
  {
    delete p_next;
    p_next = nullptr;
  }
  */
}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<(Transaction const &other)
{
  int year = this->get_year();
  int month = this->get_month();
  int day = this->get_day();
  int otheryear = other.get_year();
  int othermonth = other.get_month();
  int otherday = other.get_day();

  if (year < otheryear)
    return true;
  else if (year > otheryear)
    return false;

  if (month < othermonth)
    return true;
  else if (month > othermonth)
    return false;

  if (day < otherday)
    return true;
  else if (day > otherday)
    return false;
  else
    return false;
}
// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true : false; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb(double acb_value) { acb = acb_value; }
void Transaction::set_acb_per_share(double acb_share_value) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance(unsigned int bal) { share_balance = bal; }
void Transaction::set_cgl(double value) { cgl = value; }
void Transaction::set_next(Transaction *p_new_next) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print()
{
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
            << std::setw(4) << get_symbol() << " "
            << std::setw(4) << get_day() << " "
            << std::setw(4) << get_month() << " "
            << std::setw(4) << get_year() << " ";

  if (get_trans_type())
  {
    std::cout << "  Buy  ";
  }
  else
  {
    std::cout << "  Sell ";
  }

  std::cout << std::setw(4) << get_shares() << " "
            << std::setw(10) << get_amount() << " "
            << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
            << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
            << std::setw(10) << std::setprecision(3) << get_cgl()
            << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 3
//
History::History() // constructor a public member function which is called when the object is initialized
{
  // Default constructor. *Initializes* linked-list to be empty. To be defined by the student.
  p_head = nullptr; // beginning of the linked list is empty, hence why the p_head pointer is assigned to null pointer
}
// Destructor
// TASK 3
//
History::~History() // Destructor is an instance member function which is invoked automatically whenever an object is going to be destroyed
{
  // Destructor. To be defined by the student.
  if (this->p_head == nullptr) // destructor doesn't need to do anything if the linked list is already initialized to be empty
  {
  }
  else // linked list is not empty
  {
    Transaction *current = p_head;
    Transaction *next = p_head;
    while (true) // always enter the while loop
    {
      if (current->get_next() == nullptr) // if the next node of the list is empty, or cannot access next node
      {
        delete current;    // deallocate memory of the linked list
        current = nullptr; // delete memory of the linked list, make it empty
        break;             // break out of the while loop
      }
      // delete memory of the linked list, make it empty
      next = current->get_next();
      delete current;
      current = next;
      continue;
    }
  }
}

// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history()
{
  ece150::open_file(); // access variables and other data from the ece150 namespace

  while (ece150::next_trans_entry() == true)
  { // while the list moves to the next transaction entry
    Transaction *current_trans = new Transaction{ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount()};
    // the current transaction history is assigned to each element of the transaction history
    insert(current_trans); // using insertion sort to sort through the transaction linked list, called from void History::insert(Transaction *p_new_trans)
  }
  ece150::close_file();
}
// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans)
{ // add the passed transaction instance to the end of the linked list
  if (this->p_head == nullptr)
  {
    p_head = p_new_trans; // if the list is empty, insert transaction into linked list
  }
  else
  {
    unsigned int cur_trans_ID{p_new_trans->get_trans_id()};
    unsigned int dif_frm_HEAD{cur_trans_ID - (p_head->get_trans_id())};
    Transaction *node_before = p_head;
    for (int i{1}; i < dif_frm_HEAD; i++)
    {
      node_before = node_before->get_next();
    }
    node_before->set_next(p_new_trans);
  }
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date()
{ // sort years in chronological order first, then sort month, finally sort day, https://www.geeksforgeeks.org/insertion-sort-for-singly-linked-list/w
  Transaction *current = p_head;
  Transaction *next = nullptr; // this is set to null to initialize the next pointer to some value

  if (p_head == nullptr )
  { // if the list is already empty to begin with, it is sorted, since there is nothing to sort in an empty list
    return;
  }
  else
  {
    next = p_head->get_next();
    if(next == nullptr)//if the list only has one entry, no sorting is required
      return;
  }
if(next->get_next() == nullptr){//list contains 2 entries
  if(*current < *next)
    return;
  else{//swap entries
   p_head = next;
   next->set_next(current);
   current->set_next(nullptr);
  }
}
  while (true)
  { // while(true) ensures that the while loop always executes until break; is called
    if (*current < *next)
    { //*current is the transaction that current is pointing to, *next is the next transaction being pointed to. If *current < *next, do nothing, no sorting is required
      current = next;
      next = next->get_next(); // get the next pointer to the next node (shifting thru the list)
    }
    else
    { // if *current is greater than or equal to *next, insert *next in the right spot
      // remove the node
      current->set_next(next->get_next()); // removes the node that next is pointing to by making it no longer accessible by the list

      Transaction *temp_current = p_head;
      Transaction *temp_next = p_head->get_next();
      while (*temp_next < *next)
      { // basically check if the next node is bigger than the temp_next one, move on to the next node (node that is out of order)
        temp_current = temp_next;
        temp_next = temp_next->get_next();
      }
      // insert the node, insert *next before *temp_current, takes the removed node, assigns the address of the node to whatever temp node is pointing to, reassigns pointer inside of next (insertion)
      temp_current->set_next(next);
      next->set_next(temp_next);
      // going back to the end of the list (wherever the current pointer is currently pointing)
      next->set_next(current->get_next());
    }
    if (next == nullptr) // reach the end of the list
      break;
  }
  return;
}

  // update_acb_cgl(): Updates the ACB and CGL values.
  // TASK 7
  //
  void History::update_acb_cgl(){return;}
  

  // compute_cgl(): )Compute the ACB, and CGL.
  // TASK 8
  double History::compute_cgl(unsigned int year)
  {
  }

  // print() Print the transaction history.
  // TASK 9
  //
  void History::print()
  {
    using namespace ece150;
    std::cout << "========== BEGIN TRANSACTION HISTORY ==========" << std::endl;
    int i = 0;
    open_file();
    while (next_trans_entry() == true)
    {
      if (get_trans_type() == true)
      {
        std::cout << i << "\t" << get_trans_symbol << "\t" << get_trans_day << "\t" << get_trans_month << "\t" << get_trans_year << "\t"
                  << "Buy"
                  << "\t" << get_trans_shares << "\t" << get_trans_amount << std::endl;
      }
      else
      {
        std::cout << i << "\t" << get_trans_symbol << "\t" << get_trans_day << "\t" << get_trans_month << "\t" << get_trans_year << "\t"
                  << "Sell"
                  << "\t" << get_trans_shares << "\t" << get_trans_amount << std::endl;
      }
      std::cout << i << "\t" << get_trans_symbol << "\t" << get_trans_day << "\t" << get_trans_month << "\t" << get_trans_year << "\t" << get_trans_type << "\t" << get_trans_shares << "\t" << get_trans_amount << std::endl;
      i += 1;
    }
    std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
    close_file();
  }

  // GIVEN
  // get_p_head(): Full access to the linked list.
  //
  Transaction *History::get_p_head() { return p_head; }
