MiniSTL
====
采用C++11实现的简易STL标准库，既是C++STL的一个子集(没有实现标准IO库等)又是一个超集(增加了一些容器和算法)

目的：练习数据结构与算法和C++ Template编程

编译环境:VS2012

开发计划:
=========
* STL的几大基本组建,vector、list、deque、set、map、unordered_*等
* STL算法库中的所有算法
* AVL_set、AVL_multiset、AVL_map、AVL_multimap
* shared_ptr

完成进度:
==========
* STL的几大基本组建<br>
  * type_traits : 100% <br> 
  * pool_allocator : 100% <br>
  * iterator_traits : 100% <br>
  * reverse_iterator : 100% <br>
  * functional : 100% <br>
  * vector : 100% <br>
  * list : 100% <br>
  * string : 100% <br>
  * deque : 100% <br>
  * queue : 100% <br>
  * priority_queue : 100% <br>
  * stack : 100% <br>
  * pair : 100% <br>
  * set : 100% <br>
  * multiset : 100% <br>
  * map : 100% <br>
  * multimap : 100% <br>
  * unordered_set : 100% <br>
  * unordered_multiset : 100% <br>
  * unordered_map : 100% <br>
  * unordered_multimap : 100% <br>
* STL Algorithms:
 * adjacent_find : 100% <br> 
 * all_of : 100% <br> 
 * any_of : 100% <br> 
 * binary_search : 100% <br> 
 * copy : 100% <br> 
 * copy_backward : 100% <br> 
 * copy_if : 100% <br> 
 * copy_n : 100% <br> 
 * count : 100% <br> 
 * count_if : 100% <br> 
 * equal : 100% <br> 
 * equal_range : 100% <br> 
 * fill : 100% <br> 
 * fill_n : 100% <br> 
 * find : 100% <br> 
 * find_end : 100% <br> 
 * find_first_of : 100% <br> 
 * find_if : 100% <br> 
 * find_if_not : 100% <br> 
 * for_each : 100% <br> 
 * generate : 100% <br> 
 * generate_n : 100% <br> 
 * includes : 100% <br> 
 * inplace_merge : 100% <br> 
 * is_heap : 100% <br> 
 * is_heap_until : 100% <br> 
 * is_partitioned : 100% <br> 
 * is_permutation : 100% <br> 
 * is_sorted : 100% <br> 
 * is_sorted_until : 100% <br> 
 * iter_swap : 100% <br> 
 * lexicographical_compare : 100% <br> 
 * lower_bound : 100% <br> 
 * make_heap : 100% <br> 
 * max : 100% <br> 
 * max_element : 100% <br> 
 * merge : 100% <br> 
 * min : 100% <br> 
 * min_element : 100% <br> 
 * minmax_element : 100% <br> 
 * minmax : 100% <br> 
 * mismatch : 100% <br> 
 * move : 100% <br> 
 * move_backward : 100% <br> 
 * next_permutation : 100% <br> 
 * nth_element : 100% <br> 
 * none_of : 100% <br> 
 * partial_sort : 100% <br> 
 * partial_sort_copy : 100% <br> 
 * partition : 100% <br> 
 * partition_copy : 100% <br> 
 * pop_heap : 100% <br> 
 * prev_permutation : 100% <br> 
 * push_heap : 100% <br> 
 * random_shuffle : 100% <br> 
 * remove : 100% <br> 
 * remove_copy : 100% <br> 
 * remove_copy_if : 100% <br> 
 * replace : 100% <br> 
 * replace_copy : 100% <br> 
 * replace_copy_if : 100% <br> 
 * reverse : 100% <br> 
 * reverse_copy : 100% <br> 
 * rotate : 100% <br> 
 * rotate_copy : 100% <br> 
 * search : 100% <br> 
 * search_n : 100% <br> 
 * set_difference : 100% <br> 
 * set_intersection : 100% <br> 
 * set_symmetric_difference : 100% <br> 
 * set_union : 100% <br> 
 * sort : 100% <br> 
 * sort_heap : 100% <br> 
 * stable_partition : 100% <br> 
 * stable_sort : 100% <br> 
 * swap : 100% <br> 
 * swap_ranges : 100% <br> 
 * transform : 100% <br> 
 * unique : 100% <br> 
 * unique_copy : 100% <br> 
 * upper_bound : 100% <br> 

MiniSTL性能测试:
=================
测试环境:windows 10 && vs2012 && debug模式 && release模式
--------------------------------------------------------------
测试结果:
----------
###(1):vector<int>

int main()<br>
{<br>
  Mini_STL::vector<int> Vec;<br>
  //std::vector<int> Vec;<br>
  ULONGLONG startTime;<br>
  startTime = GetTickCount64();<br>
  for (int i=0; i!=10000000; ++i)<br>
    Vec.push_back(i);<br>
  std::cout << "Total time:"<br>
            << GetTickCount() - startTime<br>
            << std::endl;<br>
}

|container|quantity|debug time(ms)|release time(ms)|  
|---------|--------|--------|--------|  
|MiniSTL::vector&lt;int>|10万|0|0|  
|MiniSTL::vector&lt;int>|100万|30|16|  
|MiniSTL::vector&lt;int>|1000万|203|62|  
|std::vector&lt;int>|10万|0|0|  
|std::vector&lt;int>|100万|62|16| 
|std::vector&lt;int>|1000万|781|78|   

###(2):deque<int>

int main()<br>
{<br>
  Mini_STL::deque<int> idq;<br>
  //std::deque<int> idq;<br>
  ULONGLONG startTime;<br>
  startTime = GetTickCount64();<br>
  const int max = 10000000;<br>
  for (int i=0; i!=max/2; ++i)<br>
    idq.push_front(i);<br>
  for (int i=0; i!=max/2; ++i)<br>
    idq.push_back(i);<br>
  std::cout << "Total time:"<br>
            << GetTickCount() - startTime<br>
            << std::endl;<br>
}

|container|quantity|debug time(ms)|release time(ms)|  
|---------|--------|--------|--------|  
|MiniSTL::dequer&lt;int>|10万|0|0|  
|MiniSTL::deque&lt;int>|100万|16|16|  
|MiniSTL::deque&lt;int>|1000万|141|31|  
|std::deque&lt;int>|10万|16|0|  
|std::deque&lt;int>|100万|141|16| 
|std::deque&lt;int>|1000万|1328|203|   


###(3):list<int>

int main()<br>
{<br>
  Mini_STL::list<int> iList;<br>
  //std::list<int> iList;<br>
  ULONGLONG startTime;<br>
  startTime = GetTickCount64();<br>
  for (int i=0; i!=100000; ++i)<br>
    iList.push_back(i);<br>
  std::cout << "Total time:"<br>
            << GetTickCount() - startTime<br>
            << std::endl;<br>
}<br>

|container|quantity|debug time(ms)|release time(ms)|  
|---------|--------|--------|--------|  
|MiniSTL::listr&lt;int>|10万|15|0|  
|MiniSTL::list&lt;int>|100万|62|16|  
|MiniSTL::list&lt;int>|1000万|703|93|  
|std::list&lt;int>|10万|31|16|  
|std::list&lt;int>|100万|312|63| 
|std::list&lt;int>|1000万|3140|750|   

###(4):unordered_set<int>

int main()<br>
{<br>
  Mini_STL::unordered_set<int> iset;<br>
  //std::unordered_set<int> iset;<br>
  ULONGLONG startTime;<br>
  startTime = GetTickCount64();<br>
  for (size_t i=0; i!=10000; ++i)<br>
    iset.insert(rand());<br>
  std::cout << "insert time:"<br>
            << GetTickCount64() - startTime<br>
            << std::endl;<br>
  startTime = GetTickCount64();<br>
  for (size_t i=0; i!=100000000; ++i)<br>
    iset.find(rand());<br>
  std::cout << "find time:"<br>
            << GetTickCount64() - startTime<br>
            << std::endl;<br>
}

|container|quantity|insert time(ms)|query time(ms)|  
|---------|--------|--------|--------|  
|MiniSTL::unordered_set&lt;int>|1万/1亿|0|5390|  
|MiniSTL::unordered_set&lt;int>|10万/10亿|31|49360|  
|MiniSTL::unordered_set&lt;int>|100万/10亿|125|46281|  
|std::unordered_set&lt;int>|1万/1亿|0|4141|  
|std::unordered_set&lt;int>|10万/10亿|15|45110| 
|std::unordered_set&lt;int>|100万/10亿|93|42797|   

###(5):set<int>

int main()<br>
{<br>
  Mini_STL::set<int> iset;<br>
  //std::set<int> iset;<br>
  ULONGLONG startTime;<br>
  startTime = GetTickCount64();<br>
  for (size_t i=0; i!=10000; ++i)<br>
    iset.insert(rand());<br>
  std::cout << "insert time:"<br>
            << GetTickCount64() - startTime<br>
            << std::endl;<br>
  startTime = GetTickCount64();<br>
  for (size_t i=0; i!=100000000; ++i)<br>
    iset.find(rand());<br>
  std::cout << "find time:"<br>
            << GetTickCount64() - startTime<br>
            << std::endl;<br>
}

|container|quantity|insert time(ms)|query time(ms)|  
|---------|--------|--------|--------|  
|MiniSTL::unordered_set&lt;int>|1万/1亿|0|12937|  
|MiniSTL::unordered_set&lt;int>|10万/1亿|15|19328|  
|MiniSTL::unordered_set&lt;int>|100万/1亿|125|19625|  
|std::unordered_set&lt;int>|1万/1亿|0|14062|  
|std::unordered_set&lt;int>|10万/10亿|15|20641| 
|std::unordered_set&lt;int>|100万/10亿|281|21094|   

####ps : 因为采用内存池所以效率提高,但是内存池只实现了linux下加锁,故windows下不是现成安全,翻看VCstl是封装new所以是线程安全.

###(6):sort

int main()<br>
{<br>
  const size_t ARRAY_SIZE = 1000000;<br>
  char testArray[ARRAY_SIZE];<br>
  ULONGLONG startTime;<br>
  for (size_t i=0; i!=ARRAY_SIZE; ++i)<br>
    testArray[i] = rand();<br>
  startTime = GetTickCount64();<br>
  //Mini_STL::sort(testArray, testArray + ARRAY_SIZE);<br>
  std::sort(testArray, testArray + ARRAY_SIZE);<br>
  std::cout << "total time:"<br>
            << GetTickCount64() - startTime<br>
            << std::endl;<br>
}<br>


|algorithm|quantity|debug time(ms)|release time(ms)|  
|---------|--------|--------|--------|  
|MiniSTL::sort|10万|31|16|  
|MiniSTL::sort|100万|219|47|  
|std::sort|10万|15|0|  
|std::sort|100万|141|47| 
