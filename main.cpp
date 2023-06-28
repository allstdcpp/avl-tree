#include <iostream>
#include <utility>
#include <cassert>

template<typename T>
class avl_tree {
public:
    struct node {
        node* left{};
        node* right{};
        T value;
        int height{};
        int balance{};
    };

    avl_tree() = default;

    avl_tree(avl_tree&& rhs) noexcept {
        root_ = std::exchange(rhs.root_, nullptr);
        size_ = std::exchange(rhs.size_, 0);
    }

    avl_tree& operator==(avl_tree&& rhs) noexcept {
        if(this == &rhs) {
            using std::swap;

            swap(rhs.root_, root_);
            swap(rhs.size_, size_);
        }
        return *this;
    }

    template<typename F>
    void inorder_traversal(F&& f) {
        inorder_traversal(root_, std::forward<F>(f));
    }

    template<typename F>
    void preorder_traversal(F&& f) {
        preorder_traversal(root_, std::forward<F>(f));
    }

    template<typename F>
    void postorder_traversal(F&& f) {
        postorder_traversal(root_, std::forward<F>(f));
    }

    template<typename U>
    void insert(U&& v) {
        root_ = insert(root_, std::forward<U>(v));
    }

    void clear() noexcept {
        destroy(root_);
    }

    [[nodiscard]] bool contains(T const& v) const noexcept {
        auto t = root_;
        while(t) {
            if(t->value == v) {
                return true;
            }else if(v <  t->left) {
                t = t->left;
            }else{
                t = t->right;
            }
        }
        return false;
    }

    [[nodiscard]] size_t size() const noexcept {
        return size_;
    }

    [[nodiscard]] size_t height() const noexcept {
        if(!root_) return 0;
        return root_->height;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    ~avl_tree() {
        destroy(root_);
    }

private:
    template<typename F>
    void inorder_traversal(node* n, F&& f) {
        if(!n) return;

        inorder_traversal(n->left, std::forward<F>(f));
        f(n->value);
        inorder_traversal(n->right, std::forward<F>(f));
    }

    template<typename F>
    void preorder_traversal(node* n, F&& f) {
        if(!n) return;

        f(n->value);
        inorder_traversal(n->left, std::forward<F>(f));
        inorder_traversal(n->right, std::forward<F>(f));
    }

    template<typename F>
    void postorder_traversal(node* n, F&& f) {
        if(!n) return;

        inorder_traversal(n->left, std::forward<F>(f));
        inorder_traversal(n->right, std::forward<F>(f));
        f(n->value);
    }

    template<typename U>
    node* insert(node* n, U&& v)  {
        if(!n) {
            auto result = new node{nullptr, nullptr, std::forward<U>(v), 0};
            ++size_;
            return result;
        }else if(v == n->value) {
            return n;    
        }else if(v < n->value) {
            n->left = insert(n->left, std::forward<U>(v));
        }else{
            n->right = insert(n->right, std::forward<U>(v));
        }

        update(n);
        return balance(n);
    }

    void update(node* n) const noexcept {
        assert(n != nullptr);
        auto left = (n->left) ? (n->left->height) : -1;
        auto right = (n->right) ? (n->right->height) : -1;
        auto balance = right - left;

        n->height = 1 + std::max(left, right);
        n->balance = balance;
    }

    node* balance(node* n) const noexcept {
        assert(n != nullptr);
        if(n->balance == -2) { // left heavy
            if(n->left->balance <= 0) { // left left case
                return left_left_case(n);
            }
            return left_right_case(n);
        }else if(n->balance == 2) { // right heavy
            if(n->right->balance >= 0) { // right right case
                return right_right_case(n);
            }
            return right_left_case(n);
        }
        return n;
    }

    node* right_rotation(node* a) const noexcept{
        auto b = a->left;
        auto e = b->right;
        a->left = e;
        b->right = a;
        
        update(a);
        update(b);

        return b;
    }

    node* left_rotation(node* b) const noexcept {
        auto a = b->right;
        auto e = a->left;
        a->left = b;
        b->right = e;

        update(b);
        update(a);
        
        return a;
    }

    node* left_left_case(node* a) const noexcept{
        return right_rotation(a);
    }

    node* left_right_case(node* n) const noexcept {
        n->left = left_rotation(n->left);
        return left_left_case(n);
    }

    node* right_right_case(node* b) const noexcept{
        return left_rotation(b);
    }

    node* right_left_case(node* n) const noexcept {
        n->right = right_rotation(n->right);
        return right_right_case(n);
    }

    void destroy(node* n) noexcept {
        if(!n) return;

        destroy(n->left);
        destroy(n->right);

        --size_;
        delete n;
    }

    node* root_{};
    size_t size_{};
};

int main(int argc, char** argv) {

    avl_tree<int> t1;
    t1.insert(200);
    t1.insert(100);
    t1.insert(300);
    t1.insert(400);
    t1.insert(500);
    t1.insert(50);
    t1.insert(25);
    t1.insert(600);
    t1.insert(700);
    t1.insert(15);
    t1.insert(450);
    t1.insert(350);

    std::cout << std::boolalpha << "empty: " << t1.empty() << std::endl;
    std::cout << "size: " << t1.size() << std::endl;
    std::cout << "height: " << t1.height() << std::endl;

    t1.inorder_traversal([](int v){ std::cout << v << " ";});
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
