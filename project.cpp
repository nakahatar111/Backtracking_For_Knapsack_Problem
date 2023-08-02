#include <vector>
#include <climits>
#include <iostream>
#include <algorithm>
#include <queue>
#include <bits/stdc++.h>


/*
Possible other things to make
 - Searching Tree without Pruning
 - Searching Tree with Pruning
 - Other algorithms?
*/

using namespace std;

typedef struct {
  int weight;
  int profit;
  float pw;

} Item;

typedef struct {
  int weight;
  int profit;
} Bag;

typedef struct {
  int profit;
  int weight;
  float potential_profit;
  int idx;
  vector<Item> in_list;
  vector<Item> out_list;
} Node;

bool operator==(const Item &a, const Item &b) {
    return (a.weight == b.weight && a.profit == b.profit);
}
bool compare(const Item &a, const Item &b){
    return a.pw > b.pw;
}

struct Compare{
  bool operator()(const Node &a, const Node &b) const {
    return a.potential_profit < b.potential_profit;
  }
};

void updateNode(vector<Item> item_list, int weight_max, Node& node){
  int curr_weight = 0;
  float curr_profit = 0;
  node.profit = 0;
  node.weight = 0;
  for(int i = 0; i < item_list.size(); i++){
    if(find(node.out_list.begin(), node.out_list.end(), item_list[i]) == node.out_list.end()){
      if(curr_weight + item_list[i].weight < weight_max){
        curr_profit += item_list[i].profit;
        curr_weight += item_list[i].weight;
      }
      else{
        int remaining_weight = weight_max - curr_weight;
        curr_profit = remaining_weight*(item_list[i].pw) + curr_profit;
        break;
      }
    }
  }
  node.potential_profit = curr_profit;
  
  for(int i = 0; i < node.in_list.size(); i++){
    node.profit += node.in_list[i].profit;
    node.weight += node.in_list[i].weight;
  }
}

Node BestFirstSearch(vector<Item> item_list, int weight_max, int item_num, bool debug){
  priority_queue<Node, vector<Node>, Compare> queue;
  Node inital_node = {0,0,0,0,{},{}};
  updateNode(item_list, weight_max, inital_node);
  queue.push(inital_node);
  Node max_profit = {0,0,0,0,{},{}};
  int count = 0;
  while(!queue.empty()){
    Node curr_node = queue.top();
    queue.pop();
    count++;
    if(debug){
      cout << "Checking Node #" << count << endl;
      cout << "Profit: " << curr_node.profit << "\tWeight: " << curr_node.weight << "\tPotential: " << curr_node.potential_profit << endl;
    }

    if(curr_node.profit > max_profit.profit)
        max_profit = curr_node;

    if(curr_node.idx == item_num){
      continue;
    }
    Item item = item_list[curr_node.idx];
    curr_node.idx++;
    Node item_in = curr_node;
    Node item_out = curr_node;
    item_in.in_list.push_back(item);
    item_out.out_list.push_back(item);
    updateNode(item_list, weight_max, item_in);
    updateNode(item_list, weight_max, item_out);
    if(item_out.weight <= weight_max && item_out.potential_profit >= max_profit.profit)
      queue.push(item_out);
    if(item_in.weight <= weight_max && item_in.potential_profit >= max_profit.profit)
      queue.push(item_in);
  }
  if(debug)
    cout << "Checked a total of " << count << " nodes" << endl;
  return max_profit;
}

Node DepthFirstSearch(vector<Item> item_list, int weight_max, int item_num, bool prune, bool debug){
  list<Node> queue;
  Node inital_node = {0,0,0,0,{},{}};
  updateNode(item_list, weight_max, inital_node);
  queue.push_back(inital_node);
  Node max_profit = {0,0,0,0,{},{}};
  int count = 0;
  while(!queue.empty()){
    Node curr_node = queue.front();
    queue.pop_front();
    count++;
    if(debug){
      cout << "Checking Node #" << count << endl;
      cout << "Profit: " << curr_node.profit << "\tWeight: " << curr_node.weight << "\tPotential: " << curr_node.potential_profit << endl;
    }

    if(curr_node.profit > max_profit.profit && curr_node.weight <= weight_max)
        max_profit = curr_node;

    if(curr_node.idx == item_num){
      continue;
    }
    Item item = item_list[curr_node.idx];
    curr_node.idx++;
    Node item_in = curr_node;
    Node item_out = curr_node;
    item_in.in_list.push_back(item);
    item_out.out_list.push_back(item);
    updateNode(item_list, weight_max, item_in);
    updateNode(item_list, weight_max, item_out);
    if(!prune || (item_out.weight <= weight_max && item_out.potential_profit >= max_profit.profit))
      queue.push_front(item_out);
    if(!prune || (item_in.weight <= weight_max && item_in.potential_profit >= max_profit.profit))
      queue.push_front(item_in);
  }
  if(debug)
    cout << "Checked a total of " << count << " nodes" << endl;
  return max_profit;
}

Node BreadthFirstSearch(vector<Item> item_list, int weight_max, int item_num, bool prune, bool debug){
  list<Node> queue;
  Node inital_node = {0,0,0,0,{},{}};
  updateNode(item_list, weight_max, inital_node);
  queue.push_back(inital_node);
  Node max_profit = {0,0,0,0,{},{}};
  int count = 0;
  while(!queue.empty()){
    Node curr_node = queue.front();
    queue.pop_front();
    count++;
    if(debug){
      cout << "Checking Node #" << count << endl;
      cout << "Profit: " << curr_node.profit << "\tWeight: " << curr_node.weight << "\tPotential: " << curr_node.potential_profit << endl;
    }

    if(curr_node.profit >= max_profit.profit && curr_node.weight <= weight_max)
        max_profit = curr_node;

    if(curr_node.idx == item_num){
      continue;
    }
    Item item = item_list[curr_node.idx];
    curr_node.idx++;
    Node item_in = curr_node;
    Node item_out = curr_node;
    item_in.in_list.push_back(item);
    item_out.out_list.push_back(item);
    updateNode(item_list, weight_max, item_in);
    updateNode(item_list, weight_max, item_out);
    if(!prune || (item_in.weight <= weight_max && item_in.potential_profit >= max_profit.profit))
      queue.push_back(item_in);
    if(!prune || (item_out.weight <= weight_max && item_out.potential_profit >= max_profit.profit))
      queue.push_back(item_out);
  }
  if(debug)
    cout << "Checked a total of " << count << " nodes" << endl;
  return max_profit;
}

/*
        p     w
Item 1: 40    2
Item 2: 30    5
Item 3: 50    10
Item 4: 10    5
W = 16
*/

int main() {
  bool running = true;
  bool search_order = false;
  int response;
  int Weight_Limit;
  cout << "Knapsack Problem" << endl;
  cout << "Please enter the maximum weight this knapsack can hold" << endl;
  cout << "Maximum Weight: ";
  cin >> Weight_Limit;
  vector<Item> item_list;
  while(running){
    cout << endl << endl << "Below are the possible lists of commands" << endl;
    cout << "See item list (Enter 0)" << endl;
    cout << "Add an item (Enter 1)" << endl;
    cout << "Show or hide search order for node (Enter 2)" << endl;
    cout << "Run Best First Search with Branch-And-Bounding Pruning (Enter 3)" << endl;
    cout << "Run Depth First Seach Backtracking with Branch-And-Bounding Pruning (Enter 4)" << endl;
    cout << "Run Depth First Seach Backtracking without Branch-And-Bounding Pruning (Enter 5)" << endl;
    cout << "Run Breadth First Seach Backtracking with Branch-And-Bounding Pruning (Enter 6)" << endl;
    cout << "Run Breadth First Seach Backtracking without Branch-And-Bounding Pruning (Enter 7)" << endl;
    cout << "Exit (Enter -1)" << endl;
    cout << "Command: ";
    
    cin >> response;
    switch(response){
      case 0:
      {
        sort(item_list.begin(), item_list.end(), compare);
        cout << endl << "Below are the list of items" << endl;
        for(int i = 0; i < item_list.size(); i++){
          cout << "Item " << i+1 << "\tProfit: " << item_list[i].profit << "\tWeight: " << item_list[i].weight << endl;
        }
        break;
      }
      case 1:
      {
        int weight;
        int profit;
        cout << endl << "Entering Item: (Enter -1 to exit)" << endl;
        cout << "Weight: ";
        cin >> weight;
        if(weight == -1)
          break;
        cout << "Profit: ";
        cin >> profit;
        if(weight == -1)
          break;
        Item item = {weight, profit, (float)profit/weight};
        item_list.push_back(item);
        break;
      }
      case 2:
      {
        cout << endl << "Current Setting:";
        if(search_order)
          cout << "\tShowing Search Order";
        else
          cout << "\tHiding Search Order";
        cout << endl << "Enter 1 to show search order" << endl << "Enter 2 to hide search order" << endl << "Enter -1 to exit" << endl;
        cout << "Command: ";
        int setting_change;
        cin >> setting_change;
        switch(setting_change){
          case 1:
            search_order = true;
              break;
          case 2:
            search_order = false;
            break;
          default:
            break;
        }
        cout << endl << "Updated Setting:";
        if(search_order)
          cout << "\tShowing Search Order";
        else
          cout << "\tHiding Search Order";
        break;
        }
      case 3:
      {
        sort(item_list.begin(), item_list.end(), compare);
        cout << endl << "Running Best First Search on items below: (With Search Orders)" << endl;
        for(int i = 0; i < item_list.size(); i++){
          cout << "Item " << i+1 << "\tProfit: " << item_list[i].profit << "\tWeight: " << item_list[i].weight << endl;
        }
        cout << endl;
        Node max_profit = BestFirstSearch(item_list, Weight_Limit, item_list.size(), search_order);

        cout << endl << "Optimal Solution to Maximize Profit:" << endl;
        for(int j = 0; j < max_profit.in_list.size(); j++){
          cout << "Item " << (int)(find(item_list.begin(), item_list.end(), max_profit.in_list[j]) - item_list.begin()) + 1;
          if(j != max_profit.in_list.size()-1)
             cout << ", ";
          else
            cout << endl;
        }
        cout << "Total Profit: " << max_profit.profit << endl;
        cout << "Total Weight: " << max_profit.weight << endl;
        break;
      }
      case 4:
      {
        sort(item_list.begin(), item_list.end(), compare);
        cout << endl << "Running Depth First Search Backtracking on items below: (With Pruning)" << endl;
        for(int i = 0; i < item_list.size(); i++){
          cout << "Item " << i+1 << "\tProfit: " << item_list[i].profit << "\tWeight: " << item_list[i].weight << endl;
        }
        cout << endl;
        Node max_profit = DepthFirstSearch(item_list, Weight_Limit, item_list.size(),true, search_order);

        cout << endl << "Optimal Solution to Maximize Profit:" << endl;
        for(int j = 0; j < max_profit.in_list.size(); j++){
          cout << "Item " << (int)(find(item_list.begin(), item_list.end(), max_profit.in_list[j]) - item_list.begin()) + 1;
          if(j != max_profit.in_list.size()-1)
             cout << ", ";
          else
            cout << endl;
        }
        cout << "Total Profit: " << max_profit.profit << endl;
        cout << "Total Weight: " << max_profit.weight << endl;
        break;
      }
      case 5:
      {
        sort(item_list.begin(), item_list.end(), compare);
        cout << endl << "Running Depth First Search Backtracking on items below: (Without Pruning)" << endl;
        for(int i = 0; i < item_list.size(); i++){
          cout << "Item " << i+1 << "\tProfit: " << item_list[i].profit << "\tWeight: " << item_list[i].weight << endl;
        }
        cout << endl;
        Node max_profit = DepthFirstSearch(item_list, Weight_Limit, item_list.size(),false, search_order);

        cout << endl << "Optimal Solution to Maximize Profit:" << endl;
        for(int j = 0; j < max_profit.in_list.size(); j++){
          cout << "Item " << (int)(find(item_list.begin(), item_list.end(), max_profit.in_list[j]) - item_list.begin()) + 1;
          if(j != max_profit.in_list.size()-1)
             cout << ", ";
          else
            cout << endl;
        }
        cout << "Total Profit: " << max_profit.profit << endl;
        cout << "Total Weight: " << max_profit.weight << endl;
        break;
      }
      case 6:
      {
        sort(item_list.begin(), item_list.end(), compare);
        cout << endl << "Running Breadth First Search Backtracking on items below: (With Pruning)" << endl;
        for(int i = 0; i < item_list.size(); i++){
          cout << "Item " << i+1 << "\tProfit: " << item_list[i].profit << "\tWeight: " << item_list[i].weight << endl;
        }
        cout << endl;
        Node max_profit = BreadthFirstSearch(item_list, Weight_Limit, item_list.size(),true, search_order);

        cout << endl << "Optimal Solution to Maximize Profit:" << endl;
        for(int j = 0; j < max_profit.in_list.size(); j++){
          cout << "Item " << (int)(find(item_list.begin(), item_list.end(), max_profit.in_list[j]) - item_list.begin()) + 1;
          if(j != max_profit.in_list.size()-1)
             cout << ", ";
          else
            cout << endl;
        }
        cout << "Total Profit: " << max_profit.profit << endl;
        cout << "Total Weight: " << max_profit.weight << endl;
        break;
      }
      case 7:
      {
        sort(item_list.begin(), item_list.end(), compare);
        cout << endl << "Running Breadth First Search Backtracking on items below: (Without Pruning)" << endl;
        for(int i = 0; i < item_list.size(); i++){
          cout << "Item " << i+1 << "\tProfit: " << item_list[i].profit << "\tWeight: " << item_list[i].weight << endl;
        }
        cout << endl;
        Node max_profit = BreadthFirstSearch(item_list, Weight_Limit, item_list.size(),false, search_order);

        cout << endl << "Optimal Solution to Maximize Profit:" << endl;
        for(int j = 0; j < max_profit.in_list.size(); j++){
          cout << "Item " << (int)(find(item_list.begin(), item_list.end(), max_profit.in_list[j]) - item_list.begin()) + 1;
          if(j != max_profit.in_list.size()-1)
             cout << ", ";
          else
            cout << endl;
        }
        cout << "Total Profit: " << max_profit.profit << endl;
        cout << "Total Weight: " << max_profit.weight << endl;
        break;
      }
      case -1:
        running = false;
        break;
      default:
        running = false;
        break;
    }

  }
  return 0;
}
