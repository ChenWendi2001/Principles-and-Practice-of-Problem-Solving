#include "Tree.h"


// ==================== k-d tree ======================
kdNode::kdNode(){

}
kdNode::kdNode(int a[]){
	key[0]=a[0];
	key[1]=a[1];
}
kdNode::~kdNode(){
	if(left!=nullptr)delete left;
	if(right!=nullptr)delete right;
}

kdTree::kdTree(){
   root=nullptr;
}

kdTree::~kdTree(){
	if(root!=nullptr)delete root;
}

bool kdTree::equal(int a[],int b[]){
	return a[0]==b[0] && a[1]==b[1];
}

kdNode* kdTree::minNode(kdNode* a,kdNode* b,int dimCmp){
	if(b==nullptr) return a;
	if(a==nullptr) return b;
    if(a->key[dimCmp] < b->key[dimCmp]) return a;
	else return b;
}

kdNode* kdTree::maxNode(kdNode* a,kdNode* b,int dimCmp){
    if(b==nullptr) return a;
    if(a==nullptr) return b;
	if(a->key[dimCmp] > b->key[dimCmp]) return a;
	else return b;
}

void kdTree::insert(kdNode*& root, int arr[], int dim){
	if(root==nullptr){
		root = new kdNode(arr);
		return;
	}
	if(equal(root->key,arr)) return;
	if(arr[dim]<root->key[dim])
		insert(root->left,arr,(dim+1)%2);
	else
		insert(root->right,arr,(dim+1)%2);
}

kdNode* kdTree::search(kdNode* root, int arr[], int dim){
	if(root==nullptr)return NULL;
	if(equal(root->key,arr)) return root;
	if(arr[dim]<root->key[dim])
		return search(root->left,arr,(dim+1)%2);
	else
		return search(root->right,arr,(dim+1)%2);
}

void kdTree::remove(kdNode*& root, int arr[], int dim){
	if(root==nullptr)return;
   if(arr[dim]<root->key[dim])remove(root->left,arr,(dim+1)%2);
   else if(arr[dim]>root->key[dim])remove(root->right,arr,(dim+1)%2);
   else if(equal(arr,root->key)&&(root->left!=nullptr || root->right!=nullptr)){
   	if(root->right!=nullptr){
			kdNode*temp = findMin(root->right,dim,(dim+1)%2);
			root->key[0]=temp->key[0];
			root->key[1]=temp->key[1];
			remove(root->right,root->key,(dim+1)%2);
		}
		else{
			kdNode*temp = findMax(root->left,dim,(dim+1)%2);
			root->key[0]=temp->key[0];
			root->key[1]=temp->key[1];
			remove(root->left,root->key,(dim+1)%2);
		}
   }
   else{
      kdNode *tmp = root;
      root = nullptr;
      delete tmp;
   }
}

kdNode* kdTree::findMin(kdNode* root, int dimCmp, int dim){
	if(root==nullptr)return nullptr;
	kdNode* min = findMin(root->left,dimCmp,(dim+1)%2);
	if(dimCmp!=dim){
        kdNode*rightMin = findMin(root->right,dimCmp,(dim+1)%2);
		min = minNode(min,rightMin,dimCmp);
	}
	return minNode(min,root,dimCmp);
}

kdNode* kdTree::findMax(kdNode* root, int dimCmp, int dim){
	if(root==nullptr)return nullptr;
	kdNode* max = findMax(root->right,dimCmp,(dim+1)%2);
	if(dimCmp!=dim){
        kdNode*leftMax = findMax(root->left,dimCmp,(dim+1)%2);
		max = maxNode(max,leftMax,dimCmp);
	}
	return maxNode(max,root,dimCmp);
}

vector<struct kdNode*> kdTree::rangeSearch(kdNode* root, int dim, int lower_bound[], int upper_bound[]){
    vector<struct kdNode*> v;
    if(root==nullptr) {
        return v;
    }
    if(root->key[dim]<lower_bound[dim]){
        vector<struct kdNode*> v2 = rangeSearch(root->right,(dim+1)%2,lower_bound,upper_bound);
        for(int i=0; i<v2.size(); ++i) v.push_back(v2[i]);
    }
    else if(root->key[dim]>upper_bound[dim]){
        vector<struct kdNode*> v3= rangeSearch(root->left,(dim+1)%2,lower_bound,upper_bound);
        for(int i=0; i<v3.size(); ++i) v.push_back(v3[i]);
    }
    else {
        if(root->key[1-dim]<=upper_bound[1-dim] && root->key[1-dim]>=lower_bound[1-dim]) v.push_back(root);
        vector<struct kdNode*> v2 = rangeSearch(root->right,(dim+1)%2,lower_bound,upper_bound);
        for(int i=0; i<v2.size(); ++i) v.push_back(v2[i]);
        vector<struct kdNode*> v3= rangeSearch(root->left,(dim+1)%2,lower_bound,upper_bound);
        for(int i=0; i<v3.size(); ++i) v.push_back(v3[i]);
    }
    return v;
}


// ==================== trie (prefix tree) ======================
TrieNode::TrieNode() {
    flag = false;
}
TrieNode::~TrieNode() {
    map<char, TrieNode*>::iterator iter;
    for (iter=next.begin(); iter!=next.end(); iter++)
    {
        delete iter->second;
    }
}
Trie::Trie() {
    END = '$';
    root = new TrieNode();
}
Trie::~Trie(){
    delete root;
}
void Trie::insert(string word) {
    insert(word,root,0);
}
void Trie::insert(string word, TrieNode *t,int n) {
    if(t->flag){
        string word2 = t->word;
        t->flag = false;
        if(n<word2.length()){
            t->next[word2[n]]=new TrieNode();
            t->next[word2[n]]->flag = true;
            t->next[word2[n]]->word = word2;
        }
        else{
            t->next[END]=new TrieNode();
            t->next[END]->flag = true;
            t->next[END]->word = word2;
        }
        if(n<word.length())insert(word,t,n);
        else{
            t->next[END]=new TrieNode();
            t->next[END]->flag = true;
            t->next[END]->word = word;
        }
    }
    else{
        if(word.length()==n){
            t->next[END]=new TrieNode();
            t->next[END]->flag = true;
            t->next[END]->word = word;
        }
        else {
            map<char, TrieNode *>::iterator iter;
            iter = t->next.find(word[n]);
            if (iter == t->next.end()) {
                t->next[word[n]] = new TrieNode();
                t = t->next[word[n]];
                t->flag = true;
                t->word = word;
            } else {
                insert(word, iter->second, n + 1);
            }
        }
    }
}

bool Trie::search(string word) {
    return search(word,root,0);
}

bool Trie::search(string word,TrieNode* t, int n) {
    if(n==word.length()){
        map<char,TrieNode*>::iterator iter2;
        iter2 = t->next.find(END);
        if(iter2 != t->next.end())return true;
        else return false;
    }

    if(t->flag){
        return word==t->word;
    }

    map<char,TrieNode*>::iterator iter;
    iter = t->next.find(word[n]);
    if(iter == t->next.end()) return false;
    else return search(word,iter->second,n+1);
}

bool Trie::startsWith(string prefix) {
    TrieNode* t = root;
    int len = prefix.length();
    for(int i = 0;i<len+1;i++){
        if(i==len){
            return true;
        }
        if(t->flag){
            for(int j = 0;j<len;j++){
                if(t->word[j]!=prefix[j]) return false;
            }
            return true;
        }
        map<char,TrieNode*>::iterator iter;
        iter = t->next.find(prefix[i]);
        if(iter == t->next.end()) return false;
        else{
            t = iter->second;
        }
    }
}
