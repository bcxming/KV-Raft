#ifndef SKIPLIST_H
#define SKIPLIST_H
/* ************************************************************************
> File Name:     skiplist.h
> Author:        程序员Carl
> 微信公众号:    代码随想录
> Created Time:  Sun Dec  2 19:04:26 2018
> Description:
 ************************************************************************/

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <mutex>

#define STORE_FILE "store/dumpFile"

static std::string delimiter = ":";

// Class template to implement node
template <typename K, typename V>
class Node {
 public:
  Node() {}

  Node(K k, V v, int);

  ~Node();

  K get_key() const;

  V get_value() const;

  void set_value(V);

  // Linear array to hold pointers to next node of different level
  Node<K, V> **forward;

  int node_level;

 private:
  K key;
  V value;
};

template <typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
  this->key = k;
  this->value = v;
  this->node_level = level;

  // level + 1, because array index is from 0 - level
  this->forward = new Node<K, V> *[level + 1];

  // Fill forward array with 0(NULL)
  memset(this->forward, 0, sizeof(Node<K, V> *) * (level + 1));
};

template <typename K, typename V>
Node<K, V>::~Node() {
  delete[] forward;
};

template <typename K, typename V>
K Node<K, V>::get_key() const {
  return key;
};

template <typename K, typename V>
V Node<K, V>::get_value() const {
  return value;
};
template <typename K, typename V>
void Node<K, V>::set_value(V value) {
  this->value = value;
};
// Class template to implement node
template <typename K, typename V>
class SkipListDump {
 public:
  friend class boost::serialization::access;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &keyDumpVt_;
    ar &valDumpVt_;
  }
  std::vector<K> keyDumpVt_;
  std::vector<V> valDumpVt_;

 public:
  void insert(const Node<K, V> &node);
};
// Class template for Skip list
template <typename K, typename V>
class SkipList {
 public:
  SkipList(int);
  ~SkipList();
  int get_random_level();
  Node<K, V> *create_node(K, V, int);
  int insert_element(K, V);
  void display_list();
  bool search_element(K, V &value);
  void delete_element(K);
  void insert_set_element(K &, V &);
  std::string dump_file();
  void load_file(const std::string &dumpStr);
  //递归删除节点
  void clear(Node<K, V> *);
  int size();

 private:
  void get_key_value_from_string(const std::string &str, std::string *key, std::string *value);
  bool is_valid_string(const std::string &str);

 private:
  // Maximum level of the skip list
  int _max_level;

  // current level of skip list
  int _skip_list_level;

  // pointer to header node
  Node<K, V> *_header;

  // file operator
  std::ofstream _file_writer;
  std::ifstream _file_reader;

  // skiplist current element count
  int _element_count;

  std::mutex _mtx;  // mutex for critical section
};

// create new node
template <typename K, typename V>
Node<K, V> *SkipList<K, V>::create_node(const K k, const V v, int level) {
  Node<K, V> *n = new Node<K, V>(k, v, level);
  return n;
}

// Insert given key and value in skip list
// return 1 means element exists
// return 0 means insert successfully
/*
                           +------------+
                           |  insert 50 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |                      insert +----+
level 3         1+-------->10+---------------> | 50 |          70       100
                                               |    |
                                               |    |
level 2         1          10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 1         1    4     10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 0         1    4   9 10         30   40  | 50 |  60      70       100
                                               +----+

*/
template <typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value) {
  // 加锁以确保线程安全，因为多个线程可能同时插入元素
  _mtx.lock();

  // current 指向跳表的头结点，表示从头结点开始遍历跳表
  Node<K, V> *current = this->_header;

  // 创建一个 update 数组，用来存储在每一层找到的待插入位置的前驱节点
  // update[i] 将存储第 i 层的前驱节点，这些节点的 forward[i] 需要在插入后更新
  Node<K, V> *update[_max_level + 1];
  // 初始化 update 数组的所有元素为 nullptr
  memset(update, 0, sizeof(Node<K, V> *) * (_max_level + 1));

  // 从跳表的最高层开始查找合适的位置插入新节点
  for (int i = _skip_list_level; i >= 0; i--) {
    // 沿着第 i 层的 forward 指针遍历，直到找到一个比 key 大或等于的节点
    while (current->forward[i] != NULL && current->forward[i]->get_key() < key) {
      current = current->forward[i];  // 向前移动指针，直到当前节点的 key 大于等于目标 key
    }
    // 记录每一层中最后一个小于 key 的节点，之后会在这些节点后插入新节点
    update[i] = current;
  }

  // 到达第 0 层，并且 current 指向目标插入位置的后一个节点
  current = current->forward[0];

  // 如果 current 节点存在并且 key 相等，说明该 key 已经存在，不需要插入
  if (current != NULL && current->get_key() == key) {
    std::cout << "key: " << key << ", exists" << std::endl;
    _mtx.unlock();  // 解锁
    return 1;  // 返回 1 表示插入失败，因为 key 已存在
  }

  // 如果 current 是 NULL，表示已经到达了该层的末尾
  // 或者如果 current 的 key 不等于目标 key，表示新节点需要插入到 update[0] 和 current 之间
  if (current == NULL || current->get_key() != key) {
    // 为新节点生成一个随机的层数（random_level）
    int random_level = get_random_level();

    // 如果生成的 random_level 大于当前跳表的最大层数，则更新跳表的层数
    // 并且将 update 数组中比当前跳表层数高的部分初始化为头结点
    if (random_level > _skip_list_level) {
      for (int i = _skip_list_level + 1; i < random_level + 1; i++) {
        update[i] = _header;  // 设置新的层次指向头结点
      }
      _skip_list_level = random_level;  // 更新跳表的最大层数
    }

    // 创建一个具有随机层数的新节点，并插入 key 和 value
    Node<K, V> *inserted_node = create_node(key, value, random_level);

    // 将新节点插入到跳表的每一层
    for (int i = 0; i <= random_level; i++) {
      // 新节点的 forward 指向 update[i] 的 forward，即当前节点的下一个节点
      inserted_node->forward[i] = update[i]->forward[i];
      // update[i] 的 forward 更新为新插入的节点
      update[i]->forward[i] = inserted_node;
    }

    std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
    _element_count++;  // 插入成功后，元素数量增加
  }

  // 解锁
  _mtx.unlock();
  return 0;  // 返回 0 表示插入成功
}

// Display skip list
template <typename K, typename V>
void SkipList<K, V>::display_list() {
  std::cout << "\n*****Skip List*****"
            << "\n";
  for (int i = 0; i <= _skip_list_level; i++) {
    Node<K, V> *node = this->_header->forward[i];
    std::cout << "Level " << i << ": ";
    while (node != NULL) {
      std::cout << node->get_key() << ":" << node->get_value() << ";";
      node = node->forward[i];
    }
    std::cout << std::endl;
  }
}

// todo 对dump 和 load 后面可能要考虑加锁的问题
// Dump data in memory to file
template <typename K, typename V>
std::string SkipList<K, V>::dump_file() {
  // std::cout << "dump_file-----------------" << std::endl;
  //
  //
  // _file_writer.open(STORE_FILE);
  Node<K, V> *node = this->_header->forward[0];
  SkipListDump<K, V> dumper;
  while (node != nullptr) {
    dumper.insert(*node);
    // _file_writer << node->get_key() << ":" << node->get_value() << "\n";
    // std::cout << node->get_key() << ":" << node->get_value() << ";\n";
    node = node->forward[0];
  }
  std::stringstream ss;
  boost::archive::text_oarchive oa(ss);
  oa << dumper;
  return ss.str();
  // _file_writer.flush();
  // _file_writer.close();
}

// Load data from disk
template <typename K, typename V>
void SkipList<K, V>::load_file(const std::string &dumpStr) {
  // _file_reader.open(STORE_FILE);
  // std::cout << "load_file-----------------" << std::endl;
  // std::string line;
  // std::string* key = new std::string();
  // std::string* value = new std::string();
  // while (getline(_file_reader, line)) {
  //     get_key_value_from_string(line, key, value);
  //     if (key->empty() || value->empty()) {
  //         continue;
  //     }
  //     // Define key as int type
  //     insert_element(stoi(*key), *value);
  //     std::cout << "key:" << *key << "value:" << *value << std::endl;
  // }
  // delete key;
  // delete value;
  // _file_reader.close();

  if (dumpStr.empty()) {
    return;
  }
  SkipListDump<K, V> dumper;
  std::stringstream iss(dumpStr);
  boost::archive::text_iarchive ia(iss);
  ia >> dumper;
  for (int i = 0; i < dumper.keyDumpVt_.size(); ++i) {
    insert_element(dumper.keyDumpVt_[i], dumper.keyDumpVt_[i]);
  }
}

// Get current SkipList size
template <typename K, typename V>
int SkipList<K, V>::size() {
  return _element_count;
}

template <typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string &str, std::string *key, std::string *value) {
  if (!is_valid_string(str)) {
    return;
  }
  *key = str.substr(0, str.find(delimiter));
  *value = str.substr(str.find(delimiter) + 1, str.length());
}

template <typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string &str) {
  if (str.empty()) {
    return false;
  }
  if (str.find(delimiter) == std::string::npos) {
    return false;
  }
  return true;
}

// Delete element from skip list
template <typename K, typename V>
void SkipList<K, V>::delete_element(K key) {
  _mtx.lock();
  Node<K, V> *current = this->_header;
  Node<K, V> *update[_max_level + 1];
  memset(update, 0, sizeof(Node<K, V> *) * (_max_level + 1));

  // start from highest level of skip list
  for (int i = _skip_list_level; i >= 0; i--) {
    while (current->forward[i] != NULL && current->forward[i]->get_key() < key) {
      current = current->forward[i];
    }
    update[i] = current;
  }

  current = current->forward[0];
  if (current != NULL && current->get_key() == key) {
    // start for lowest level and delete the current node of each level
    for (int i = 0; i <= _skip_list_level; i++) {
      // if at level i, next node is not target node, break the loop.
      if (update[i]->forward[i] != current) break;

      update[i]->forward[i] = current->forward[i];
    }

    // Remove levels which have no elements
    while (_skip_list_level > 0 && _header->forward[_skip_list_level] == 0) {
      _skip_list_level--;
    }

    std::cout << "Successfully deleted key " << key << std::endl;
    delete current;
    _element_count--;
  }
  _mtx.unlock();
  return;
}

/**
 * \brief 作用与insert_element相同类似，
 * insert_element是插入新元素，
 * insert_set_element是插入元素，如果元素存在则改变其值
 */
template <typename K, typename V>
void SkipList<K, V>::insert_set_element(K &key, V &value) {
  V oldValue;
  if (search_element(key, oldValue)) {
    delete_element(key);
  }
  insert_element(key, value);
}

// Search for element in skip list
/*
                           +------------+
                           |  select 60 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |
level 3         1+-------->10+------------------>50+           70       100
                                                   |
                                                   |
level 2         1          10         30         50|           70       100
                                                   |
                                                   |
level 1         1    4     10         30         50|           70       100
                                                   |
                                                   |
level 0         1    4   9 10         30   40    50+-->60      70       100
*/
template <typename K, typename V>
bool SkipList<K, V>::search_element(K key, V &value) {
  std::cout << "search_element-----------------" << std::endl;
  Node<K, V> *current = _header;

  // start from highest level of skip list
  for (int i = _skip_list_level; i >= 0; i--) {
    while (current->forward[i] && current->forward[i]->get_key() < key) {
      current = current->forward[i];
    }
  }

  // reached level 0 and advance pointer to right node, which we search
  current = current->forward[0];

  // if current node have key equal to searched key, we get it
  if (current and current->get_key() == key) {
    value = current->get_value();
    std::cout << "Found key: " << key << ", value: " << current->get_value() << std::endl;
    return true;
  }

  std::cout << "Not Found Key:" << key << std::endl;
  return false;
}

template <typename K, typename V>
void SkipListDump<K, V>::insert(const Node<K, V> &node) {
  keyDumpVt_.emplace_back(node.get_key());
  valDumpVt_.emplace_back(node.get_value());
}

// 构造跳表
template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level) {
  // 初始化跳表的最大层数为 max_level
  this->_max_level = max_level;

  // 初始化当前跳表的层数为 0，表示跳表当前还没有任何元素（即仅有一层）
  this->_skip_list_level = 0;

  // 初始化跳表中的元素数量为 0
  this->_element_count = 0;

  // 创建头结点，用于在每一层的最前端，作为跳表的起点
  // 头结点的 key 和 value 初始化为空的键和值（默认构造的K和V类型）
  K k;  // 初始化空键
  V v;  // 初始化空值

  // 分配内存为头结点，层数为 _max_level
  // 头结点是特殊的节点，存放在每一层的起点，用于指向跳表的第一个元素
  this->_header = new Node<K, V>(k, v, _max_level);
};

template <typename K, typename V>
SkipList<K, V>::~SkipList() {
  if (_file_writer.is_open()) {
    _file_writer.close();
  }
  if (_file_reader.is_open()) {
    _file_reader.close();
  }

  //递归删除跳表链条
  if (_header->forward[0] != nullptr) {
    clear(_header->forward[0]);
  }
  delete (_header);
}
template <typename K, typename V>
void SkipList<K, V>::clear(Node<K, V> *cur) {
  if (cur->forward[0] != nullptr) {
    clear(cur->forward[0]);
  }
  delete (cur);
}

template <typename K, typename V>
int SkipList<K, V>::get_random_level() {
  int k = 1;
  while (rand() % 2) {
    k++;
  }
  k = (k < _max_level) ? k : _max_level;
  return k;
};
// vim: et tw=100 ts=4 sw=4 cc=120
#endif  // SKIPLIST_H
