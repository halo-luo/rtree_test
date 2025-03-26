#ifndef __data_structure__
#define __data_structure__

#include <vector>
#include <deque>
#include <list>
#include <unordered_map>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <chrono>
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/polygon/polygon.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
namespace gtl = boost::polygon;

typedef bg::model::point<int, 2, bg::cs::cartesian> point_type;
typedef bg::model::box<point_type> box_type;
typedef bg::model::polygon<point_type> polygon_type;
typedef std::pair<box_type, int> value;


typedef gtl::polygon_90_data<int> BoostPolygon;
typedef gtl::polygon_traits<BoostPolygon>::point_type BoostPolygonTraits;
typedef gtl::rectangle_data<int> BoostRectangle;
typedef gtl::point_data<int> BoostPoint;


typedef std::pair<box_type, BoostPolygon> BoostPolygonValue;
typedef std::pair<box_type,BoostRectangle> BoostRectangleValue;

typedef bgi::rtree<BoostPolygonValue, bgi::quadratic<16>> RTree;
typedef bgi::rtree<BoostRectangleValue, bgi::quadratic<16>> RTreeRect;


typedef bg::model::multi_polygon<polygon_type> multi_polygon_type;

using namespace std;

enum MODE {
	ACC, ECC
};

typedef std::pair<int, int> pint;;



// 整数型点
class Point {
public:
	int x, y;

	Point() {};

	Point(int x, int y) : x(x), y(y) {};
	Point(const Point& p) : x(p.x), y(p.y) {};

	bool operator==(const Point& p) const {
		return x == p.x && y == p.y;
	};

	bool operator!=(const Point& p) const {
		return x != p.x || y != p.y;
	};

	bool operator<(const Point& p) const {
		if (x != p.x) return x < p.x;
		return y < p.y;
	};

	Point operator+(const Point& p) const {
		return Point(x + p.x, y + p.y);
	};

	Point operator-(const Point& p) const {
		return Point(x - p.x, y - p.y);
	};

};

// 双精度浮点型点
class DPoint {
public:
	double x, y;

	bool operator==(const DPoint& p) const {
		return x == p.x && y == p.y;
	};
	bool operator!=(const DPoint& p) const {
		return x != p.x || y != p.y;
	};
	bool operator<(const DPoint& p) const {
		if (x != p.x) return x < p.x;
		return y < p.y;
	};
	bool operator <=(const DPoint& p) const {
		if (x != p.x) return x <= p.x;
		return y <= p.y;
	};
};

// 矩形
struct Rect {
	Point ll, ru;

	Rect() {};

	Rect(const Point& ll, const Point& ru) {
		this->ll = ll;
		this->ru = ru;
	};

	Rect(int x0, int y0, int x1, int y1) {
		this->ll.x = x0;
		this->ll.y = y0;
		this->ru.x = x1;
		this->ru.y = y1;
	};

};

// 多边形
class Polygon {
public:
	std::vector<Point> points;

	Rect boundingBox;// 多边形的边界框
	DPoint center; // 多边形的中心点
	int ID;// 多边形的ID，记录在整个版图中的编号

	int layer = 0;
	int datatype = 0;

	void initLLRU() {
		// 计算多边形的边界框
		if (this->points.empty()) return;

		Point lowerLeft(INT_MAX, INT_MAX);
		Point upperRight(INT_MIN, INT_MIN);
		for (const auto& point : this->points) {
			lowerLeft.x = std::min(lowerLeft.x, point.x);
			lowerLeft.y = std::min(lowerLeft.y, point.y);
			upperRight.x = std::max(upperRight.x, point.x);
			upperRight.y = std::max(upperRight.y, point.y);
		}
		this->boundingBox.ll = lowerLeft;
		this->boundingBox.ru = upperRight;
	}

	void initCenter() {
		// 计算多边形的中心点，需要先初始化boundingBox
		this->center.x = (this->boundingBox.ll.x + this->boundingBox.ru.x) / 2.0;
		this->center.y = (this->boundingBox.ll.y + this->boundingBox.ru.y) / 2.0;
	}

	// 打印多边形顶点坐标
	/*void print() const {
		std::cout << "Polygon points: ";
		if (points.empty()) {
			std::cout << "Empty polygon." << std::endl;
			return;
		}
		for (const auto& point : points) {
			std::cout << "(" << point.x << "," << point.y << ") ";
		}
		std::cout << std::endl;
	}*/

	// 设置多边形的编号
	void setID(int ID) {
		this->ID = ID;
	}
};


class Layout {
public:
	unsigned short layer; // 版图层号id
	unsigned short datatype; // 版图由layer和datatype确定哪一层

	Rect boundingBox; // 版图的边界框
	//std::vector<Polygon*> polygons; // 版图中的所有多边形
	// 版图中的所有多边形，使用智能指针管理内存
	//std::vector<Polygon*> polygons; // 版图中的所有多边形
	std::vector<std::unique_ptr<Polygon>> polygons; // 版图中的所有多边形

	void initLLRU() {
		// 计算版图的边界框
		if (this->polygons.empty()) return;

		Point lowerLeft(INT_MAX, INT_MAX);
		Point upperRight(INT_MIN, INT_MIN);
		for (const auto& polygon : this->polygons) {
			lowerLeft.x = std::min(lowerLeft.x, polygon->boundingBox.ll.x);
			lowerLeft.y = std::min(lowerLeft.y, polygon->boundingBox.ll.y);
			upperRight.x = std::max(upperRight.x, polygon->boundingBox.ru.x);
			upperRight.y = std::max(upperRight.y, polygon->boundingBox.ru.y);
		}
		this->boundingBox.ll = lowerLeft;
		this->boundingBox.ru = upperRight;
	};

	void centerXY_sort() {
		// 按照中心点的y坐标排序
		/*std::sort(this->polygons.begin(), this->polygons.end(),
			[](const std::unique_ptr<Polygon>& p1, const std::unique_ptr<Polygon>& p2) {
				return p1->center < p2->center;
			});*/
	}

	~Layout() {};
};

// a clip is a set of polygons and a marker
class Clip {
public:
	Rect marker;
	int markerType; //the type of the marker defined by NCS rule
	Point Center;
	int pointNum;

	Rect bbox; // 剪切片的边界框
	box_type box_geo;
	box_type s_box_geo;// 小剪辑的边界框
	BoostPolygon boost_bbox; // 剪切片的边界框，用于boost库的计算
	BoostRectangle boost_bbox_rect;

	std::vector<BoostPolygon> clip_polygons;// 剪辑中包含的多边形
	std::vector<box_type> clip_rects; // 大剪辑列表
	std::vector<box_type> s_clip_rects; // 小剪辑列表

	int area = 0;


	~Clip() {};



	// 生成topo
	/*void initTopo() {
		std::set<int> x_set, y_set;
		for (const auto& poly : bPolygons) {
			for (const auto& pt : poly->pt_list) {
				x_set.insert(pt.x);
				y_set.insert(pt.y);
			}
		}

		std::vector<int> x_vector(x_set.begin(), x_set.end());
		std::vector<int> y_vector(y_set.begin(), y_set.end());



		std::map<int, int> x_map, y_map;
		for (int i = 0; i < x_vector.size(); i++)
			x_map[x_vector[i]] = i;
		for (int i = 0; i < y_vector.size(); i++)
			y_map[y_vector[i]] = i;


	};*/



};


class Topo {
public:
	std::vector<int> x_list , y_list; // 记录所有x坐标和y坐标的列表
	
	std::map<int,int> x_map, y_map; // 记录x坐标和y坐标的映射关系

	pint inner_length;
	std::vector<int> x_dim_list, y_dim_list; // 记录所有x维度和y维度的列表

	std::vector<std::vector<int>> grid; // 记录每个格子的多边形ID

	std::string coder;

	// 实现print函数
	/*void print() const {
		std::cout << "Topo:" << std::endl;
		std::cout << "x_list: ";
		for (const auto& x : x_list) {
			std::cout << x << " ";
		}
		std::cout << std::endl;
		std::cout << "y_list: ";
		for (const auto& y : y_list) {
			std::cout << y << " ";
		}
		std::cout << std::endl;
		std::cout << "x_map: ";
		for (const auto& x : x_map) {
			std::cout << x.first << ":" << x.second << " ";
		}
		std::cout << std::endl;
		std::cout << "y_map: ";
		for (const auto& y : y_map) {
			std::cout << y.first << ":" << y.second << " ";
		}
		std::cout << std::endl;

		std::cout << "inner_length: " << inner_length.first;
		std::cout << "  " << inner_length.second << std::endl;

		std::cout << "x_dim_list: ";
		for (const auto& x : x_dim_list) {
			std::cout << x << " ";
		}
		std::cout << std::endl;
		std::cout << "y_dim_list: ";
		for (const auto& y : y_dim_list) {
			std::cout << y << " ";
		}
		std::cout << std::endl;

		std::cout << "grid: " << std::endl;
		for (const auto& row : grid) {
			for (const auto& cell : row) {
				std::cout << cell << " ";
			}
			std::cout << std::endl;
		}
	};*/
};


class Work {
public:
	MODE mode; // 工作模式，ACC或ECC

	double acc = 1.0;
	int pWidth, pHeight;
	int ecc = 0;

	char output_gds[100];
	std::vector<std::unique_ptr<Clip>> clips; // 所有剪切片
	int N = 0; // 剪切片数量

	Work() {
		this->mode = MODE::ACC;
		this->pWidth = 200;
		this->pHeight = 200;

		this->acc = 1.0;
	}

	void test_index(Layout* layout) {
		std::cout << layout[0].polygons.size() << std::endl;
		std::cout << layout[1].polygons.size() << std::endl;
		// 记录开始时间点
		auto start = std::chrono::high_resolution_clock::now();

		// 预分配内存优化
		//std::vector<std::unique_ptr<Polygon>>& polygons = layout[1].polygons;
		auto& polygons = layout[1].polygons;
		const size_t poly_count = polygons.size();
		std::vector<std::pair<box_type, size_t>> rtree_entries;
		rtree_entries.reserve(poly_count); // 预分配vector空间

		for (size_t i = 0; i < poly_count; ++i) {

			// 减少重复计算
			const auto& bb = polygons[i]->boundingBox; // 本地引用
			rtree_entries.emplace_back(
				box_type(point_type(bb.ll.x, bb.ll.y),
					point_type(bb.ru.x, bb.ru.y)),
				i
			);
		}

		// 批量构建R树 (速度提升关键)
		bgi::rtree<value,bgi::quadratic<128>> rtree(rtree_entries.begin(), rtree_entries.end());


		// 记录结束时间点
		auto end = std::chrono::high_resolution_clock::now();

		// 计算时间差（支持多种时间单位转换）
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::cout << "耗时: " << duration.count() << " 微秒" << std::endl;
	}

	void ext_clip(Layout* layout) {

		int clipNum = layout[1].polygons.size();

		
		// 计算剪切片的边界框
		int radius_x = this->pWidth / 2;
		int radius_y = this->pHeight / 2;
		for (size_t i = 0; i < clipNum; i++) {
			layout[1].polygons[i]->initLLRU();
			layout[1].polygons[i]->initCenter();

			this->clips.push_back(std::make_unique<Clip>());
			this->clips[i]->marker.ll = layout[1].polygons[i]->boundingBox.ll;
			this->clips[i]->marker.ru = layout[1].polygons[i]->boundingBox.ru;
			this->clips[i]->markerType = 0;
			this->clips[i]->Center.x = layout[1].polygons[i]->center.x;
			this->clips[i]->Center.y = layout[1].polygons[i]->center.y;


			this->clips[i]->bbox.ll = layout[1].polygons[i]->boundingBox.ll - Point(100, 100);
			this->clips[i]->bbox.ru = layout[1].polygons[i]->boundingBox.ru + Point(100, 100);

			std::vector<BoostPolygonTraits> tmp_points = {
				gtl::construct<BoostPolygonTraits>(this->clips[i]->bbox.ll.x, this->clips[i]->bbox.ll.y),
				gtl::construct<BoostPolygonTraits>(this->clips[i]->bbox.ru.x, this->clips[i]->bbox.ll.y),
				gtl::construct<BoostPolygonTraits>(this->clips[i]->bbox.ru.x, this->clips[i]->bbox.ru.y),
				gtl::construct<BoostPolygonTraits>(this->clips[i]->bbox.ll.x, this->clips[i]->bbox.ru.y),
			};
			gtl::set_points(this->clips[i]->boost_bbox, tmp_points.begin(), tmp_points.end());
			
			this->clips[i]->boost_bbox_rect = gtl::construct<BoostRectangle>(
				this->clips[i]->bbox.ll.x, this->clips[i]->bbox.ll.y,
				this->clips[i]->bbox.ru.x, this->clips[i]->bbox.ru.y);

			int x1 = this->clips[i]->bbox.ll.x;
			int y1 = this->clips[i]->bbox.ll.y;
			int x2 = this->clips[i]->bbox.ru.x;
			int y2 = this->clips[i]->bbox.ru.y;

			this->clips[i]->box_geo = box_type(
				point_type(this->clips[i]->bbox.ll.x, this->clips[i]->bbox.ll.y),
				point_type(this->clips[i]->bbox.ru.x, this->clips[i]->bbox.ru.y));

			this->clips[i]->s_box_geo = box_type(
				point_type(this->clips[i]->Center.x - radius_x, this->clips[i]->Center.y - radius_y),
				point_type(this->clips[i]->Center.x + radius_x, this->clips[i]->Center.y + radius_y));

		}



		// 获取多边形的边界框，方便后续建立索引
		//bgi::rtree<polygon_type, bgi::linear<16>> rtree;

		// 为版图构建索引
		// 创建 R 树，二次分割算法
		bgi::rtree<value, bgi::quadratic<16>> rtree; // 插入多边形外接框
		bgi::rtree<box_type, bgi::quadratic<16>> rtree_clip; // 插入矩形
		/*
		* 
		typedef std::pair<box_type, BoostPolygon> BoostPolygonValue;
		typedef std::pair<box_type,BoostRectangle> BoostRectangleValue;
		typedef bgi::rtree<BoostPolygonValue, bgi::quadratic<16>> RTree;
		typedef bgi::rtree<BoostRectangleValue, bgi::quadratic<16>> RTreeRect;
		*/
		RTree rtree_polygon;
		RTreeRect rtree_clip_rect;

		// 构造R树
		for (size_t i = 0; i < layout[0].polygons.size(); i++) {
			layout[0].polygons[i]->initLLRU();
			layout[0].polygons[i]->setID(i);

			rtree.insert(std::make_pair(box_type(
				point_type(
					layout[0].polygons[i]->boundingBox.ll.x,
					layout[0].polygons[i]->boundingBox.ll.y),
				point_type(
					layout[0].polygons[i]->boundingBox.ru.x,
					layout[0].polygons[i]->boundingBox.ru.y)
			), i));
			

			BoostPolygon tmp_polygon;
			std::vector<BoostPolygonTraits> tmp_points;
			for (size_t j = 1; j < layout[0].polygons[i]->points.size(); j++) {
				tmp_points.push_back(
					gtl::construct<BoostPolygonTraits>(layout[0].polygons[i]->points[j].x, layout[0].polygons[i]->points[j].y)
				);
			}
			gtl::set_points(tmp_polygon, tmp_points.begin(), tmp_points.end());
			std::vector<BoostRectangle> tmp_rectangles;

			gtl::get_rectangles(tmp_rectangles, tmp_polygon);

			for (BoostRectangle& rect : tmp_rectangles) {
				int x1 = gtl::xl(rect);
				int y1 = gtl::yl(rect);
				int x2 = gtl::xh(rect);
				int y2 = gtl::yh(rect);

				rtree_clip.insert(box_type(
					point_type(x1, y1),
					point_type(x2, y2))
				);

				rtree_clip_rect.insert(std::make_pair(
					box_type(point_type(x1, y1),
						point_type(x2, y2)),
					gtl::construct<BoostRectangle>(x1, y1, x2, y2)
				));
			}

			rtree_polygon.insert(
				std::make_pair(box_type(
					point_type(
						layout[0].polygons[i]->boundingBox.ll.x,
						layout[0].polygons[i]->boundingBox.ll.y),
					point_type(
						layout[0].polygons[i]->boundingBox.ru.x,
						layout[0].polygons[i]->boundingBox.ru.y)
				), tmp_polygon)
			);

		}
		
		// 创建查询窗口
		box_type queryBox(point_type(133608, 271696), point_type(133679, 271728));

		// 执行查询
		std::vector<value> result;
		rtree.query(bgi::intersects(queryBox), std::back_inserter(result));


		// 打印查询结果
		std::cout << "Query results:" << std::endl;
		for (auto& item : result) {
			std::cout << "Box index: " << item.second << std::endl;

			// 
			std::cout << item.first.min_corner().get<0>() << " " << item.first.min_corner().get<1>() << " " << item.first.max_corner().get<0>() << " " << item.first.max_corner().get<1>() << std::endl;
		}

		// 查询
		for (size_t i = 0; i < layout[1].polygons.size(); i++) {
			int x1 = this->clips[i]->bbox.ll.x;
			int y1 = this->clips[i]->bbox.ll.y;
			int x2 = this->clips[i]->bbox.ru.x;
			int y2 = this->clips[i]->bbox.ru.y;

			queryBox = box_type(point_type(x1, y1), point_type(x2, y2));
			this->clips[i]->box_geo = queryBox;

			std::vector<value> result;
			std::vector<box_type> result_clip;
			rtree.query(bgi::intersects(queryBox), std::back_inserter(result));

			for (auto& item : result) {
				//std::cout << "Box index: " << item.second << std::endl;

				// 
				//std::cout << item.first.min_corner().get<0>() << " " << item.first.min_corner().get<1>() << " " << item.first.max_corner().get<0>() << " " << item.first.max_corner().get<1>() << std::endl;
			}

			rtree_clip.query(bgi::intersects(queryBox), std::back_inserter(result_clip));


			std::vector<box_type> intersections_box;
			//std::vector<polygon_type> intersect_polygons;
			box_type tmp_box;
			for (auto& item : result_clip) {
				std::cout << "Box index: " << i << std::endl;

				// 打印item
				std::cout << bg::wkt(item) << std::endl;
				
				bg::intersection(this->clips[i]->box_geo, item, tmp_box);
				intersections_box.push_back(tmp_box);

				// 求tmp_box的面积
				int area = bg::area(tmp_box);
				std::cout << "Area: " << area << std::endl;
				this->clips[i]->area += area;

			}
			this->clips[i]->clip_rects = intersections_box;

			//std::vector<BoostPolygonValue> intersect_polygons;
			/*std::vector<BoostPolygon> result_polygons;
			rtree_polygon.query(bgi::intersects(queryBox), std::back_inserter(intersect_polygons));*/
			//std::vector<BoostPolygon> polygon_intersections;
			//for (auto& item : intersect_polygons) {
			//	
			//	bg::intersection(this->clips[i]->boost_bbox, item.second, polygon_intersections);
			//	std::cout << "Polygon intersection size: " << polygon_intersections.size() << std::endl;
			//}

			

		}
		
		/*map<int,vector<int>> clip_map;
		for (size_t i = 0; i < this->clips.size(); i++) {
			clip_map[this->clips[i]->area].push_back(i);
		}
		std::cout << "Clip map size: " << clip_map.size() << std::endl;
		for (auto& item : clip_map) {
			std::cout << "Area: " << item.first << std::endl;
			for (auto& i : item.second) {
				std::cout << "Clip index: " << i << std::endl;
			}
		}*/


	}


	void test() {

		// 交集，使用multi_polygon_type，求取的是两个多边形的交集，两个多边形序列的交集
		// 具体做法是先求第一个set1的交集，然后求第二个set2的交集，最后求两个交集的交集

		multi_polygon_type set1, set2;
		polygon_type poly1;
		//std::vector<point_type> points0 = { {0, 0},  {0, 100}, {100, 100},{100, 0},{0,0} };
		std::vector<point_type> points0 = { {-100,0},{0,0},{0,100},{-100,100},{-100,0} };
		poly1.outer().assign(points0.begin(), points0.end());
		bg::correct(poly1);
		//bg::exterior_ring(poly1) = { {0, 0}, {100, 0}, {100, 100}, {0, 100},{0,0} };
		set1.push_back(poly1);

		//std::vector<point_type> points1 = { {1, 1}, {2, 1}, {2, 2}, {1, 2}, {1, 1} };
		std::vector<point_type> points1 = { {-10,0},{100,0},{100,100},{-10,100},{-10,0} };
		polygon_type poly3;
		poly3.outer().assign(points1.begin(), points1.end());
		bg::correct(poly3);
		set1.push_back(poly3);
		
		polygon_type poly2;
		//std::vector<point_type> points2 = { {0, 0}, {0, -1},{-1, -1},{-1, 0},  {0,0} };
		//std::vector<point_type> points2 = { {1, 0}, {2, 0}, {2, 1}, {1, 1},{1,0} };
		std::vector<point_type> points2 = { {-100, 10}, {-100, -100}, {0, -100}, {0, 10},{-100,10} };
		poly2.outer().assign(points2.begin(), points2.end());
		bg::correct(poly2);
		//bg::exterior_ring(poly2) = { {1, 0}, {2, 0}, {2, 1}, {1, 1},{1,0} };
		set2.push_back(poly2);
		//set2.push_back(poly1);

		polygon_type poly4;
		std::vector<point_type> points4 = { {-10, 10}, {-10, -100}, {100, -100}, {100, 10},{-10,10} };
		poly4.outer().assign(points4.begin(), points4.end());
		bg::correct(poly4);
		set2.push_back(poly4);

		multi_polygon_type result;
		//vector<polygon_type> result_polygons;
		bg::intersection(set1, set2, result);
		//bg::union_(poly1, poly2, result_polygons);
		//std::cout << "Union size: " << result_polygons.size() << std::endl;

		std::cout << "Intersection size: " << result.size() << std::endl;
		for (auto& poly : result) {
			std::cout << bg::wkt(poly) << std::endl;
		}

		std::cout << "set1 size: " << set1.size() << std::endl;
		for (auto& poly : set1) {
			std::cout << bg::wkt(poly) << std::endl;
		}

		std::cout << "set2 size: " << set2.size() << std::endl;
		for (auto& poly : set2) {
			std::cout << bg::wkt(poly) << std::endl;
		}
	}


	void test2() {

		multi_polygon_type set1, set2;
		polygon_type poly1;
		std::vector<point_type> points0 = { {-100,0},{0,0},{0,100},{-100,100},{-100,0} };
		poly1.outer().assign(points0.begin(), points0.end());
		bg::correct(poly1);
		set1.push_back(poly1);


		polygon_type poly2;
		std::vector<point_type> points2 = { {-100, 10}, {-100, -100}, {0, -100}, {0, 10} };
		poly2.outer().assign(points2.begin(), points2.end());
		bg::correct(poly2);
		set2.push_back(poly2);


		multi_polygon_type result;
		vector<polygon_type> result_polygons;
		bg::union_(poly1, poly2, result_polygons);
		bg::union_(set1, set2, result);

		std::cout << "Union size: " << result_polygons.size() << std::endl;
		std::cout << "" << result.size() << std::endl;

		for (auto& poly : result) {
			std::cout << bg::wkt(poly) << std::endl;
		}

		std::cout << "set1 size: " << set1.size() << std::endl;
		for (auto& poly : set1) {
			std::cout << bg::wkt(poly) << std::endl;
		}

		std::cout << "set2 size: " << set2.size() << std::endl;
		for (auto& poly : set2) {
			std::cout << bg::wkt(poly) << std::endl;
		}
	}

	void test3() {

		box_type box1(point_type(0, 0), point_type(100, 100));
		box_type box2(point_type(100, 100), point_type(150, 150));


		box_type result;
		bg::intersection(box1, box2, result);
		
		// 判断result是否合法
		std::cout << bg::wkt(result) << std::endl;
	}

};



#endif
