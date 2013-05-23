#ifndef SPC_COMMON_H
#define SPC_COMMON_H



#include <vector>
#include <cmath>
#include <iostream>
#include <utility>


#include <spc/methods/rbf_interpolator.h>

#include <Eigen/Core>

namespace spc
{

template<typename nType> std::vector<nType> 
subdivideRange(nType &start, nType &end, nType &step);

template<typename nType> std::vector<int>
get_nans_id(const std::vector<nType> vector)
{
	std::vector<int> nans_id;
	for (int i = 0; i < vector.size(); ++i)
	{
		if (isnan(vector[i]))
		{
			nans_id.push_back(i);
		}
	}
	return nans_id;
}

template<typename nType> std::vector<nType>
erease_ids(const std::vector<nType> &Vector, const std::vector<int> Ids)
{
	//sort indexes
	std::vector<int> ids = Ids;
	std::vector<nType> vector = Vector;
	sort (ids.begin(), ids.end());
	
	//remove from the higher to lower, so indexes are not changed
	for(int i=ids.size() - 1; i >= 0; i--){
		vector.erase(vector.begin() + ids[i]);
	}
	return vector;
}

template<typename nType> void
fill_nan_rbf(std::vector<nType> &x, std::vector<nType> &y)
{
	//get nans ids
	std::vector<int> nans_id = get_nans_id(y);
	
	
	for (int i = 0; i < nans_id.size(); ++i)
	{
	}
	
	//remove this ids from the original dataset
	std::vector<nType> x_stripped = erease_ids(x, nans_id);
	std::vector<nType> y_stripped = erease_ids(y, nans_id);
	
	//now create a rbf interpolator
    spc::RBFInterpolator<nType> rbf;
	rbf.setInputPoints(x_stripped);
	rbf.setInputValues(y_stripped);
	rbf.updateAll();
	
	
	
	for (int i = 0; i < nans_id.size(); ++i)
	{
		int this_id = nans_id[i];
		nType this_x_position = x[this_id];
		Eigen::Matrix<nType, Eigen::Dynamic, 1> x_point;
		x_point.resize(1);
		x_point[0] = this_x_position;
		y[this_id] = rbf.evaluateRbf(x_point);
	}
	
	//now for each nan evaluate the rbf
	
	
}


// void
// fill_nan_rbf(std::vector<float> &x, std::vector<float> &y)
// {
//
// 	//remove nans from y
// 	RBFInterpolator<float> rbf;
// 	Eigen::VectorXf a;
// 	Eigen::VectorXf b;
//
// 	//count nans
// 	int n_nans = 0;
// 	for (int i = 0; i < n; ++i)
// 	{
// 		float value = y[i];
// 		if (isnan(value))
// 		{
// 			n_nans += 1;
// 		}
// 	}
//
// 	for (int i = 0; i < n_nans; ++i)
// 	{
//
// 		a(i) = x[i]
// 		float value = y[i];
// 		if (!isnan(value))
// 		{
// 			a(i) = y[i];
// 			b
// 		}
//
// 	}
// }

/// TO USE WITH ++11 support
//template<typename Stype>
//std::vector<int>
//getSortingIds(std::vector<Stype> const &v)
//{
//    using iter = decltype(v.begin());
//    using pair = std::pair<size_t, iter>;

//    std::vector<pair> order;
//    order.reserve(v.size());

//    size_t n = 0;
//    for (auto const& e : v)
//        order.push_back = std::make_pair(n++, e);

//    std::sort(order.begin(), order.end(), [](pair const& a, pair const& b) { return *a.second < *b.second; });
//    std::vector<int> out(order.size());
//    std::transform(begin(order), end(order), begin(out), [](pair const& p) { return p.first; });
//    return out;
//}


template<typename Stype>
std::vector<int>
getSortingIds(const std::vector<Stype> &v)
{
//    typedef typename std::vector<Stype>::const_iterator myiter;
//    typedef typename std::pair<size_t, myiter> pairT ;

    using myiter = decltype(v.begin());
    using pairT = std::pair<size_t, myiter>;

    std::vector<pairT> order;

    size_t n = 0;
    for (auto const &e : v)
        order.push_back ( std::make_pair(n++, (myiter) &e  ) );

    std::sort(order.begin(), order.end(),  [](pairT const& a, pairT const& b) { return *(a.second) < *(b.second); });

    std::vector<int> out(order.size());
    std::transform(order.begin(), order.end(), out.begin(), [](  pairT  &pair) { return pair.first; } );
    return out;
}


template <typename Stype>
std::vector<int>
getFirstNSortingIds(const std::vector<Stype> &v, const int N)
{
    std::vector<int> all_ids = getSortingIds(v);
    std::vector<int> out(all_ids.begin(), all_ids.begin() + N);
    return out;

}

template <typename Stype>
std::vector<int>
getLastNSortingIds(const std::vector<Stype> &v, const int N)
{

    std::vector<int> all_ids = getSortingIds(v);
    std::vector<int> out(all_ids.end() - N, all_ids.end());
//    std::transform(end(all_ids) - N, end(all_ids), begin(out), [](int &val){return val;} );

    return out;

}


}//end namespace
#endif