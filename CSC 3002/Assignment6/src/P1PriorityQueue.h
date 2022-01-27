/*
 * File: pqueue.h
 * --------------
 * This file exports the PriorityQueue class, a collection in which values
 * are processed in priority order.
 */

#ifndef _pqueue_h
#define _pqueue_h

#include "vector.h"
#include "error.h"
using namespace std;

/*
 * Class: PriorityQueue<ValueType>
 * -------------------------------
 * This class models a structure called a priority queue in which values
 * are processed in order of priority.  As in conventional English usage,
 * lower priority numbers correspond to higher effective priorities,
 * so that a priority 1 item takes precedence over a priority 2 item.
 */
int PriorityQueueTest();

template<typename ValueType>
class PriorityQueue {

 public:

/*
 * Constructor: PriorityQueue
 * Usage: PriorityQueue<ValueType> pq;
 * -----------------------------------
 * Initializes a new priority queue, which is initially empty.
 */

  PriorityQueue();

/*
 * Destructor: ~PriorityQueue
 * --------------------------
 * Frees any heap storage associated with this priority queue.
 */

  virtual ~PriorityQueue();

/*
 * Method: size
 * Usage: int n = pq.size();
 * -------------------------
 * Returns the number of values in the priority queue.
 */

  int size() const;

/*
 * Method: isEmpty
 * Usage: if (pq.isEmpty()) ...
 * ----------------------------
 * Returns true if the priority queue contains no elements.
 */

  bool isEmpty() const;

/*
 * Method: clear
 * Usage: pq.clear();
 * ------------------
 * Removes all elements from the priority queue.
 */

  void clear();

/*
 * Method: enqueue
 * Usage: pq.enqueue(value, priority);
 * -----------------------------------
 * Adds value to the queue with the specified priority.
 * Lower priority numbers correspond to higher priorities,
 * which means that all
 * priority 1 elements are dequeued before any priority 2 elements.
 */

  void enqueue(ValueType value, double priority);

/*
 * Method: dequeue
 * Usage: ValueType first = pq.dequeue();
 * --------------------------------------
 * Removes and returns the highest priority value.
 * If multiple entries in
 * the queue have the same priority,
 * those values are dequeued in the same
 * order in which they were enqueued.
 */

  ValueType dequeue();

/*
 * Method: peek
 * Usage: ValueType first = pq.peek();
 * -----------------------------------
 * Returns the value of highest priority in the queue, without removing it.
 */

  ValueType peek() const;

/*
 * Method: peekPriority
 * Usage: double priority = pq.peekPriority();
 * -------------------------------------------
 * Returns the priority of the first element in the queue,
 * without removing it.
 */

  double peekPriority() const;

/*
 * Method: toString
 * Usage: string str = pq.toString();
 * ----------------------------------
 * Converts the queue to a printable string representation.
 */

  std::string toString();

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

/*
 * Implementation notes: PriorityQueue data structure
 * --------------------------------------------------
 * The PriorityQueue class is implemented using a data structure called a
 * heap.
 */

 private:

/* Type used for each heap entry */

  struct HeapEntry {
    ValueType value;

    double priority;

    long sequence;
    /* order of enqueue, which used for recording the order of which the elements are inserted,
    /* i.e.: when two elements are of the same priority,
     * it depends on the order of the sequence that
     * the first inserted element should be dequeued first
     */

  };

/* Instance variables */

  Vector<HeapEntry> heap;
  long enqueueCount; // to record the sequence
  int count;         // to record the number of elements in the PriorityQueue
  int capacity;      // capacity of the heap

/* Private function prototypes */

  void enqueueHeap(ValueType &value, double priority);
  ValueType dequeueHeap();
  bool takesPriority(int i1, int i2);
  void swapHeapEntries(int i1, int i2);

};

template<typename ValueType>
PriorityQueue<ValueType>::PriorityQueue() {
  clear();
}

/*
 * Implementation notes: ~PriorityQueue destructor
 * -----------------------------------------------
 * All of the dynamic memory is allocated in the Vector class, so no work
 * is required at this level.
 */

template<typename ValueType>
PriorityQueue<ValueType>::~PriorityQueue() {
  /* Empty */
}

template<typename ValueType>
int PriorityQueue<ValueType>::size() const {
  return count;
}

template<typename ValueType>
bool PriorityQueue<ValueType>::isEmpty() const {
  // return true if nothing
  if (count == 0)
    return true;
  return false;
}

template<typename ValueType>
void PriorityQueue<ValueType>::clear() {
  // reinitialize and set initial capacity
  heap.clear();
  capacity = 10000;
  heap = Vector<HeapEntry>(10000);
  count = 0;
}

template<typename ValueType>
void PriorityQueue<ValueType>::enqueue(ValueType value, double priority) {
  // assign new value
  heap[count++].value = value;
  heap[count - 1].priority = priority;
  heap[count - 1].sequence = count;
  if (count == 1) return; // nothing to do
    // rearrange
  else {
    int index = count - 1;
    while (index != 0 && takesPriority(index, (index - 1) / 2)) {
      swapHeapEntries(index, (index - 1) / 2);
      index = (index - 1) / 2;
    }
  }
}

/*
 * Implementation notes: dequeue, peek, peekPriority
 * -------------------------------------------------
 * These methods must check for an empty queue and report an error if there
 * is no first element.
 */

template<typename ValueType>
ValueType PriorityQueue<ValueType>::dequeue() {
  if (count == 0) // empty test
    error("There is no first element!");
  ValueType out = heap[0].value;
  count--;
  swapHeapEntries(0, count);
  int index = 0;
  int left = 1, right = 2;//index of the left and right child of the node
  while ((left < count)) { // arrange only in scope
    if (takesPriority(left, index)) { // left must swap
      if (right < count && takesPriority(right, left)) { //right must swap
        swapHeapEntries(index, right);
        index = right;
      } else { // only left need to swap
        swapHeapEntries(index, left);
        index = left;
      }
    } else if ((right < count) && takesPriority(right, index)) { // only right need to swap
      swapHeapEntries(index, right);
      index = right;
    } else {
      break;// no need to swap
    }
    left = 2 * index + 1;
    right = 2 * index + 2;
  }
  return out;
}

template<typename ValueType>
ValueType PriorityQueue<ValueType>::peek() const {
  if (count == 0) //test
    error("There is no first element!");
  HeapEntry* temp = new HeapEntry; // set pointer
  temp->value = heap[0].value;
  return temp->value;
}

template<typename ValueType>
double PriorityQueue<ValueType>::peekPriority() const {
  return heap[0].priority;
}

template<typename ValueType>
bool PriorityQueue<ValueType>::takesPriority(int i1, int i2) {
  /* compare the priority of two elements，
  /* when their priorities are equal, comparing based on the sequence */
  if (heap[i1].priority < heap[i2].priority) { //case 1
    return true;
  } else if (heap[i1].priority > heap[i2].priority) { //case 2
    return false;
  } else { // further compare
    if (heap[i1].sequence < heap[i2].sequence)
      return true;
    else return false;
  }
}

template<typename ValueType>
void PriorityQueue<ValueType>::swapHeapEntries(int i1, int i2) {
  // create buffer and swap
  int priority = heap[i1].priority;
  ValueType value = heap[i1].value;
  int sequence = heap[i1].sequence;
  heap[i1].priority = heap[i2].priority;
  heap[i1].value = heap[i2].value;
  heap[i1].sequence = heap[i2].sequence;
  heap[i2].priority = priority;
  heap[i2].value = value;
  heap[i2].sequence = sequence;
}

template<typename ValueType>
std::string PriorityQueue<ValueType>::toString() {
  /* convert the PriorityQueue into a printable String */
  string out = "";
  int len = heap.size();
  for (int i = 0; i < len; i++)
    out += heap[i].value;
  return out;
}

template<typename ValueType>
std::ostream &operator<<(std::ostream &os,
                         const PriorityQueue<ValueType> &pq) {
  /* give the output of all the elements in queue(not dequeue)
  /* i.e.: cout << pq gives the output of all the content in current queue without change pq */
  for (int i = 0; i < pq.count; i++) {
    os << pq[i].value << " ";
  }
  return os;
}

#endif
