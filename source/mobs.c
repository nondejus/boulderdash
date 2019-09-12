#include <SDL2/SDL.h>
#include "mobs.h"
#include "fundamentals.h"
#include "blocks.h"

int check_player_around(Block ** map, Point s);

int check_player_around(Block ** map, Point s){
  if(get_side(map,s,_left)==player) return -1;
  if(get_side(map,s,_right)==player) return -1;
  if(get_side(map,s,_up)==player) return -1;
  if(get_side(map,s,_down)==player) return -1;
  return 1;
}

int try_move_two_way(Block ** map, Point m, Point p, Direction d1, Direction d2){
  if (get_side(map, m,d1) == empty || get_side(map, m, d1) == player){
    if(get_side(map, m, d1) == player){
      block_move(map, m, d1);
      return -1;
    }
    block_move(map, m, d1);
    return 1;
  }
  else if (get_side(map, m, d2) == empty || get_side(map, m, d2) == player){
    if(get_side(map, m, d2) == player){
      block_move(map, m, d2);
      return -1;
    }
    block_move(map, m, d2);
    return 1;
  }
  else return 0;
}

int monster_move(Block **map, Point m, Point p){
  int disH, disV;
  disH = p.x - m.x;
  disV = p.y - m.y;
  if (abs(disH) < abs(disV)){
    if (disV < 0 && disH < 0)
      return try_move_two_way(map,m,p,_up,_left);
    else if (disV < 0)
      return try_move_two_way(map,m,p,_up,_right);
    else if (disH < 0)
      return try_move_two_way(map,m,p,_down,_left);
    else
      return try_move_two_way(map,m,p,_down,_right);
  }
  else{
    if (disV < 0 && disH < 0)
      return try_move_two_way(map,m,p,_left,_up);
    else if (disV < 0)
      return try_move_two_way(map,m,p,_right,_up);
    else if (disH < 0)
      return try_move_two_way(map,m,p,_left,_down);
    else
      return try_move_two_way(map,m,p,_right,_down);
  }
}

int spider_move(Block **map, Point s){
  if(get_side(map, s, _right)!=empty&&get_side(map, s, _right)!=player&&
     get_side(map, s, _down)!=empty&&get_side(map, s, _down)!=player&&
     get_side(map, s, _left)!=empty&&get_side(map, s, _left)!=player&&
     get_side(map, s, _up)!=empty&&get_side(map, s, _up)!=player) return 0;
  switch(map[s.y][s.x].active){
    case 0:
      if (get_side(map, s, _right) == empty){
        map[s.y][s.x].active=_right;
        block_move(map, s, _right);
        return check_player_around(map,side_pos(s,_right));
      }
      if (get_side(map, s, _right) == player){
        map[s.y][s.x].active=_right;
        block_move(map, s, _right);
        return -1;
      }
      map[s.y][s.x].active=_right;
      return spider_move(map,s);
    case _right:
      if (get_side(map, s, _right) == empty){
        map[s.y][s.x].active=_right;
        block_move(map, s, _right);
        return check_player_around(map,side_pos(s,_right));
      }
      if (get_side(map, s, _right) == player){
        map[s.y][s.x].active=_right;
        block_move(map, s, _right);
        return -1;
      }
      map[s.y][s.x].active=_down;
      return spider_move(map,s);
    case _down:
      if (get_side(map, s, _down) == empty){
        map[s.y][s.x].active=_down;
        block_move(map, s, _down);
        return check_player_around(map,side_pos(s,_down));
      }
      if (get_side(map, s, _down) == player){
        map[s.y][s.x].active=_down;
        block_move(map, s, _down);
        return -1;
      }
      map[s.y][s.x].active=_left;
      return spider_move(map,s);
    case _left:
      if (get_side(map, s, _left) == empty){
        map[s.y][s.x].active=_left;
        block_move(map, s, _left);
        return check_player_around(map,side_pos(s,_left));
      }
      if (get_side(map, s, _left) == player){
        map[s.y][s.x].active=_left;
        block_move(map, s, _left);
        return -1;
      }
      map[s.y][s.x].active=_up;
      return spider_move(map,s);
    case _up:
      if (get_side(map, s, _up) == empty){
        map[s.y][s.x].active=_up;
        block_move(map, s, _up);
        return check_player_around(map,side_pos(s,_up));
      }
      if (get_side(map, s, _up) == player){
        map[s.y][s.x].active=_up;
        block_move(map, s, _up);
        return -1;
      }
      map[s.y][s.x].active=_right;
      return spider_move(map,s);
    default: return -1;
  }
}

void create_diamonds(Block **map, Point coords, block_t t, int mapW, int mapH){
  Point c = coords;
  c.y += 1;
  switch (t){
    case monster:
      if (c.y < mapH - 3 && c.y > 2 && c.x < mapW - 3 && c.x > 2){
        if (get_block_xy(map,c.x,c.y-2)!=border && get_block_xy(map,c.x,c.y-2)!=player)
          set_block_xy(map,c.x,c.y-2,diamond);
        if (get_block_xy(map,c.x,c.y+2)!=border && get_block_xy(map,c.x,c.y+2)!=player)
          set_block_xy(map,c.x,c.y+2,diamond);
        if (get_block_xy(map,c.x-2,c.y)!=border && get_block_xy(map,c.x-2,c.y)!=player)
          set_block_xy(map,c.x-2,c.y,diamond);
        if (get_block_xy(map,c.x+2,c.y)!=border && get_block_xy(map,c.x+2,c.y)!=player)
          set_block_xy(map,c.x+2,c.y,diamond);

        if (get_block_xy(map,c.x,c.y-1)!=border && get_block_xy(map,c.x,c.y-1)!=player)
          set_block_xy(map,c.x,c.y-1,diamond);
        if (get_block_xy(map,c.x,c.y+1)!=border && get_block_xy(map,c.x+2,c.y+1)!=player)
          set_block_xy(map,c.x,c.y+1,diamond);
        if (get_block_xy(map,c.x-1,c.y)!=border && get_block_xy(map,c.x-1,c.y)!=player)
          set_block_xy(map,c.x-1,c.y,diamond);
        if (get_block_xy(map,c.x+1,c.y)!=border && get_block_xy(map,c.x+1,c.y)!=player)
          set_block_xy(map,c.x+1,c.y,diamond);
        if (get_block_xy(map,c.x-1,c.y-1)!=border && get_block_xy(map,c.x-1,c.y-1)!=player)
          set_block_xy(map,c.x-1,c.y-1,diamond);
        if (get_block_xy(map,c.x+1,c.y-1)!=border && get_block_xy(map,c.x+1,c.y-1)!=player)
          set_block_xy(map,c.x+1,c.y-1,diamond);
        if (get_block_xy(map,c.x-1,c.y+1)!=border && get_block_xy(map,c.x-1,c.y+1)!=player)
          set_block_xy(map,c.x-1,c.y+1,diamond);
        if (get_block_xy(map,c.x+1,c.y+1)!=border && get_block_xy(map,c.x+1,c.y+1)!=player)
          set_block_xy(map,c.x+1,c.y+1,diamond);
      }
      break;
    case spider:
      if (get_block_xy(map,c.x,c.y-1)==dirt)
        set_block_xy(map,c.x,c.y-1,diamond);
      if (get_block_xy(map,c.x,c.y+1)==dirt)
        set_block_xy(map,c.x,c.y+1,diamond);
      if (get_block_xy(map,c.x-1,c.y)==dirt)
        set_block_xy(map,c.x-1,c.y,diamond);
      if (get_block_xy(map,c.x+1,c.y)==dirt)
        set_block_xy(map,c.x+1,c.y,diamond);
      if (get_block_xy(map,c.x-1,c.y-1)==dirt)
        set_block_xy(map,c.x-1,c.y-1,diamond);
      if (get_block_xy(map,c.x+1,c.y-1)==dirt)
        set_block_xy(map,c.x+1,c.y-1,diamond);
      if (get_block_xy(map,c.x-1,c.y+1)==dirt)
        set_block_xy(map,c.x-1,c.y+1,diamond);
      if (get_block_xy(map,c.x+1,c.y+1)==dirt)
        set_block_xy(map,c.x+1,c.y+1,diamond);
      break;
    default: break;
  }
}
