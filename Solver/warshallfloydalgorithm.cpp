#include "warshallfloydalgorithm.h"

WarshallFloydAlgorithm::WarshallFloydAlgorithm(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    std::tie(size_x, size_y) = field.getSize();
    size_sum = size_x * size_y;

    dock = std::make_shared<ProgresDock>();
    dock->show();
}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> WarshallFloydAlgorithm::agentAct(int){

    std::vector<std::pair<int,int>> agent_pos = {field.getAgent(side, 0), field.getAgent(side, 1)};

    std::vector<std::vector<Edge>> edges = calcDijkStra(getPosValue(agent_pos.at(0)), 30);
    std::vector<std::vector<std::list<std::pair<int,int>>>> routes;

    std::map<std::pair<int,int>, MapElement> route_map;

    Treap treap_que;

    for(int pos = 0; pos < size_sum; ++pos)
        for(int depth = 1; depth < 30; ++depth){
            double score;
            std::list<std::pair<int,int>> route;

            std::tie(score, route) = getRoute(edges, pos, depth);
            if(route.size() < 2)
                continue;
            treap_que.insert(std::make_pair(score / depth, std::move(route)));

        }

    treap_que.erase_back(1.0 * treap_que.size() * bound_val);

    for(int index = 0; index < treap_que.size(); ++index){
        double average;
        std::list<std::pair<int,int>> route;

        std::tie(average, route) = treap_que.get(index);

        std::cout << average << std::endl;

        std::pair<int,int> back = *std::next(route.begin());

        MapElement element(back);

        if(route_map.find(back) == route_map.end())
            route_map[back] = MapElement(back);
        route_map[back].addRoute(average, std::move(route));

    }

    for(auto& map_element : route_map){

        map_element.second.setPutCount(field.getSize());

        const std::pair<int,int>& target_pos = map_element.second.pos;
        const std::vector<std::list<std::pair<int,int>>> routes = map_element.second.routes.second;
        const std::vector<std::vector<int>>& putcounts = map_element.second.put_count;

        std::vector<std::vector<std::vector<int>>> color(3, std::vector<std::vector<int>>(size_x, std::vector<int>(size_y, 255)));

        color.at(0).at(target_pos.first).at(target_pos.second) = 128;
        color.at(1).at(agent_pos.at(0).first).at(agent_pos.at(0).second) = 128;
        color.at(2).at(agent_pos.at(0).first).at(agent_pos.at(0).second) = 128;

        int put_count_sum = 0.0;
        std::for_each(putcounts.begin(), putcounts.end(), [&put_count_sum](const std::vector<int>& v){put_count_sum += std::accumulate(v.begin(), v.end(), 0);});

        for(int point = 0; point < size_sum; ++point){
            std::pair<int,int> point_pair = getPosPair(point);

            if(point_pair == target_pos)
                continue;
            color.at(1).at(point_pair.first).at(point_pair.second) -= putcounts.at(point_pair.first).at(point_pair.second) * 512 / put_count_sum;
            color.at(2).at(point_pair.first).at(point_pair.second) -= putcounts.at(point_pair.first).at(point_pair.second) * 512 / put_count_sum;
        }

        dock->addMinumuVisu(field.getSize(), routes, color);
    }

    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));
}

std::pair<double, std::list<std::pair<int,int>>> WarshallFloydAlgorithm::getRoute(std::vector<std::vector<Edge>>& inp_2ut, int target_pos, int depth){

    auto get_list = [&](int pos, int depth){

        std::list<std::pair<int, int>> pos_list;

        if(inp_2ut.at(pos).at(depth).depth != depth){
            // std::cerr << "route is not found" << std::endl;
            return pos_list;
        }

        while(depth > 0){
            pos_list.emplace_back(getPosPair(pos));

            int bef_pos = pos;
            pos = inp_2ut.at(pos).at(depth).prev.first;
            depth = inp_2ut.at(bef_pos).at(depth).prev.second;
        }

        pos_list.emplace_back(getPosPair(pos));

        pos_list.reverse();

        return pos_list;
    };

    std::list<std::pair<int,int>> routes;
    routes = get_list(target_pos, depth);

    // dock->addMinumuVisu(field.getSize(), std::vector<std::list<std::pair<int,int>>>({routes}), std::vector<std::vector<std::vector<int>>>(3, std::vector<std::vector<int>>(size_x, std::vector<int>(size_y, 255))));

    return std::make_pair(inp_2ut.at(target_pos).at(depth).sum, routes);
}

std::vector<std::vector<WarshallFloydAlgorithm::Edge>> WarshallFloydAlgorithm::calcDijkStra(int start_pos, int maxval){

    std::vector<std::vector<Edge>> dp_vector(size_sum);

    for(int start_index = 0; start_index < size_sum; ++start_index)
        for(int end_index = 0; end_index < maxval; ++end_index)
            dp_vector.at(start_index).emplace_back(start_index);

    dp_vector.at(start_pos).at(0).depth = 0;

    auto check = [&](int start_pos, int depth, int end_pos){
        while(depth > 0){
            if(start_pos == end_pos)
                return true;

            int bef_pos = start_pos;
            start_pos = dp_vector.at(start_pos).at(depth).prev.first;
            depth = dp_vector.at(bef_pos).at(depth).prev.second;
        }

        return start_pos == end_pos;
    };

    for(int depth = 0; depth < maxval; ++depth)
        for(int point = 0; point < size_sum; ++point)
            if(dp_vector.at(point).at(depth).depth == depth)
                for(int direction = 0; direction < 8; ++direction)
                    if(!outOfRange(point, direction)){

                        int end_index = getRotatePos(point, direction);

                        if(check(point, depth, end_index))
                            continue;

                        std::pair<int, int> end_pos = getPosPair(end_index);
                        std::pair<int, int> end_pos_state = field.getState(end_pos.first, end_pos.second);

                        int length = 1 + (end_pos_state.first == side ? 1 : 2);
                        double value = (end_pos_state.first == side + 1 ? 0 : end_pos_state.second * length);

                        if(depth + length >= maxval)
                            continue;

                        Edge e = Edge::make(dp_vector.at(point).at(depth), end_index, value, length);

                        dp_vector.at(end_index).at(depth + length) = std::max(dp_vector.at(end_index).at(depth + length), e);
                    }

    return dp_vector;
}

std::pair<int, int> WarshallFloydAlgorithm::getPosPair(int x){
    return std::make_pair(x / size_y, x % size_y);
}

int WarshallFloydAlgorithm::getPosValue(std::pair<int, int> pos){
    return size_y * pos.first + pos.second;
}

int WarshallFloydAlgorithm::getRotatePos(int pos, int rotate){
    return pos + size_y * dx.at(rotate) + dy.at(rotate);
}

bool WarshallFloydAlgorithm::outOfRange(int pos, int rotate){

    std::pair<int, int> field_size = field.getSize();
    std::pair<int,int> pos_pair = getPosPair(pos);
    pos_pair.first += dx.at(rotate);
    pos_pair.second += dy.at(rotate);

    return pos_pair.first < 0 || pos_pair.second < 0 || pos_pair.first >= field_size.first || pos_pair.second >= field_size.second;
}

const std::vector<int> WarshallFloydAlgorithm::dx({1, 1, 0, -1, -1, -1, 0, 1, 0});
const std::vector<int> WarshallFloydAlgorithm::dy({0, -1, -1, -1, 0, 1, 1, 1, 0});



WarshallFloydAlgorithm::Treap::Treap() : root(TreapNode::nil){}

WarshallFloydAlgorithm::Treap::Treap(tr_type val) : root(std::make_shared<TreapNode>(val)){}

// イテレータが指す[st,en)の範囲で初期化する
WarshallFloydAlgorithm::Treap::Treap(std::vector<tr_type>::iterator st, std::vector<tr_type>::iterator en) : root(TreapNode::nil){
    while(st != en){
        root = _merge(root, std::make_shared<TreapNode>(*st));
        ++st;
    }
}

// 配列で初期化する
WarshallFloydAlgorithm::Treap::Treap(std::vector<tr_type> v) : root(TreapNode::nil){
    for(auto& xx : v)
        root = _merge(root, std::make_shared<TreapNode>(xx));
}

int WarshallFloydAlgorithm::Treap::_size(np_2 x){
    return x == TreapNode::nil ? 0 : x->size;
}

np_2 WarshallFloydAlgorithm::Treap::_update(np_2 x){
    x->size = _size(x->l) + _size(x->r) + 1;
    return x;
}

np_2 WarshallFloydAlgorithm::Treap::_merge(np_2 l, np_2 r){
    if(l == TreapNode::nil || r == TreapNode::nil)
        return l == TreapNode::nil ? r : l;

    if(l->pri > r->pri){
        l->r = _merge(l->r, r);
        return _update(l);
    }else{
        r->l = _merge(l, r->l);
        return _update(r);
    }
}

std::pair<np_2,np_2> WarshallFloydAlgorithm::Treap::_split(np_2 x, int k){
    if(x == TreapNode::nil)
        return std::make_pair(TreapNode::nil, TreapNode::nil);

    if(k <= _size(x->l)){
        std::pair<np_2,np_2> s = _split(x->l, k);
        x->l = s.second;
        return std::make_pair(s.first, _update(x));

    }else{
        std::pair<np_2,np_2> s = _split(x->r, k - _size(x->l) - 1);
        x->r = s.first;
        return std::make_pair(_update(x), s.second);
    }
}

np_2 WarshallFloydAlgorithm::Treap::_insert(np_2 x, int k, tr_type val){
    np_2 l,r;
    std::tie(l, r) = _split(x, k);
    return _merge(_merge(l, std::make_shared<TreapNode>(val)), r);
}

np_2 WarshallFloydAlgorithm::Treap::_erase(np_2 x, int k){
    np_2 l,r,m;
    std::tie(l, r) = _split(x, k);
    std::tie(m, r) = _split(r, 1);
    return _merge(l, r);
}

np_2 WarshallFloydAlgorithm::Treap::_erase_back(np_2 x, int k){
    return _split(x, k).first;
}

void WarshallFloydAlgorithm::Treap::_set(np_2 x, int k, tr_type val){
    if(k < _size(x->l))
        _set(x->l, k, val);
    else if(_size(x->l) == k)
        x->val = val;
    else
        _set(x->r, k - _size(x->l) - 1, val);

    _update(x);
}

tr_type WarshallFloydAlgorithm::Treap::_get(np_2 x, int k){
    if(k < _size(x->l))
        return _get(x->l, k);
    else if(_size(x->l) == k)
        return x->val;
    else
        return _get(x->r, k - _size(x->l) - 1);
}

int WarshallFloydAlgorithm::Treap::_lowerbound(np_2 x, tr_type val){
    if(x == TreapNode::nil)
        return 0;
    if(val >= x->val)
        return _lowerbound(x->l, val);
    else
        return _lowerbound(x->r,val) + _size(x->l) + 1;
}

np_2 WarshallFloydAlgorithm::Treap::_insert(np_2 x, tr_type val){
    return _insert(x, _lowerbound(x, val), val);
}

void WarshallFloydAlgorithm::Treap::push_front(tr_type val){
    root = _merge(std::make_shared<TreapNode>(val), root);
}

void WarshallFloydAlgorithm::Treap::push_back(tr_type val){
    root = _merge(root, std::make_shared<TreapNode>(val));
}

void WarshallFloydAlgorithm::Treap::pop_front(){
    root = _split(root, 1).second;
}

void WarshallFloydAlgorithm::Treap::pop_back(){
    root = _split(root, _size(root) - 1).first;
}

int WarshallFloydAlgorithm::Treap::size(){
    return root == TreapNode::nil ? 0 : root->size;
}

void WarshallFloydAlgorithm::Treap::set(int k, tr_type val){
    return _set(root, k, val);
}

tr_type WarshallFloydAlgorithm::Treap::get(int k){
    return _get(root, k);
}

void WarshallFloydAlgorithm::Treap::insert(int k, tr_type val){
    root = _insert(root, k, val);
}

void WarshallFloydAlgorithm::Treap::insert(tr_type val){
    root = _insert(root, val);
}

int WarshallFloydAlgorithm::Treap::lowerbound(tr_type val){
    return _lowerbound(root, val);
}

void WarshallFloydAlgorithm::Treap::erase(int k){
    root = _erase(root, k);
}

void WarshallFloydAlgorithm::Treap::erase_back(int k){
    root = _erase_back(root, k);
}

np_2 WarshallFloydAlgorithm::TreapNode::nil = std::make_shared<WarshallFloydAlgorithm::TreapNode>(tr_type(), 0);
