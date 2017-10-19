//
// Created by yidafu on 2017/10/15.
//

#include <cstring>
#include <iostream>
#include "language.h"
#include "Stack.h"

/// 实现规则一，输入一个字符串，由一个队列将结果带回
/// \param str 要转换的字符串
/// \param trans_result 经过规则一转换的结果队列
void rule_one(char *str,Queue *trans_result) {

    extern char beelzebubWorlds[2][6];
    int len = strlen(str);
    Deque first_trans;

    // 放入队列
    for (int index = 0; index < len; ++index) {
        first_trans.enQueue(str[index]);
    }

    char ch;
    while ( ! first_trans.isEmpty() ) {
        first_trans.delQueue(ch);
        if ( 'A' <= ch && ch <= 'Z') {
            char *beelzebub_world = beelzebubWorlds[ ch % 65];
            for (int index = strlen( beelzebub_world) - 1; index >= 0; index--) {
                first_trans.enFront( beelzebub_world[index] );
            }
        } else {
            trans_result->enQueue(ch);
        }
    }
}

/// 实现规则一的另一个范本
/// \param beelzebubWorld A-Z 魔王的词汇
/// \return 从魔王的词汇翻译过来的字符串指针
char * rule_one_v1(char &beelzebubWorld) {
    extern char beelzebubWorlds[2][6];
    int world_no = beelzebubWorld % 65;

    char result[100] ={},*world =  beelzebubWorlds[world_no];

    int str_len = strlen(world);

//    TODO 目前能够解析的深度为2
    for (int char_count = 0; char_count < str_len; char_count ++ ) {
        if( world[ char_count ] >= 'A' && world[ char_count ] <= 'Z') {
            strcat( result, beelzebubWorlds[ world[ char_count ] % 65] );
        }else{
            result[ strlen(result) ] = world[char_count];
        }
    }
    char *p = result;
    return p;
}

/// 实际上实现规则二的函数
/// \param str 括号里面的字符串
/// \return 规则二转换的结果字符串
char * real_rule_two ( char *str ) {
    char needle = str[0];
    Stack *encode = new Stack;
    encode->push(needle);
    for( int index = 1; index < strlen(str) ; index ++ ) {
        encode->push(str[index]);
        encode->push(needle);
    }
    // 转换成字符串方便返回，返回局部对象的情况不好处理
    char *ret_str,ch;
    ret_str = ( ElemType * ) malloc( encode->length() * sizeof(ElemType) + 1);
    int index = 0;
    while ( ! encode->isEmpty() ) {
        encode->pop(ch);
        ret_str[index] = ch;
        index ++;
    }
    // 字符串末尾加上 \0
    ret_str[ index ++ ] = '\0';

    delete encode;
    return ret_str;
}

/// 把括号里面的字符串找到，传入 real_rule_two() 函数
/// \param trans_result trans_result 要转换的字符串队列
/// \param result result 返回的结果队列
void rule_two(Queue *trans_result,Queue *result){
    Stack brackets,in_brackets;
    char rule_two_str[50],
            ch,
        clapboard = '|'; // 隔板
    int index = 0;
    while ( ! trans_result->isEmpty() ) {
        trans_result->delQueue(ch);
        // assic 41 = )
        if( ch == 41 ) {
            brackets.pop();

            /// 两种情况
            /// 1. 一层括号了 --->>> 空了
            /// 2. 多层括号  --->>> 未空

            // 将隔板上面的字母 pop
            char get;
            Stack to_trans;
            do {
                in_brackets.pop( get );
                to_trans.push(get);
            } while (get != clapboard && ! in_brackets.isEmpty() );


            if( get == '|' ) {
                // pop 隔板
                to_trans.pop();
            }

            char needle;
            to_trans.pop( needle );
            in_brackets.push( needle );
            while ( ! to_trans.isEmpty() ) {
                to_trans.pop( get );
                in_brackets.push( get );
                in_brackets.push( needle );
            }
            char en_queue;
            if ( brackets.isEmpty() ) {
                while ( ! in_brackets.isEmpty() ) {
                    in_brackets.pop( en_queue );
                    result->enQueue( en_queue );
                }
            }

//            char *strp = real_rule_two(rule_two_str);
            // 将字符压入队列
//            for (int index = 0; index < strlen(strp); ++index) {
//                result->enQueue(strp[index]);
//            }
            // rule_two_str 的索引清零
//            index = 0;
            // 结束括号跳过，不加入队列
            continue;
        }

        // assic 40 = (
        if( ch == 40 ) {
            // 遇到 ( 并且 brackets 不空，就在 in_brackets 压入一个隔板 |
            if ( ! brackets.isEmpty() ) {
                in_brackets.push( clapboard );
            }
            brackets.push(ch);
            // 开始括号跳过不加入队列
//            result->enQueue( clapboard );
            continue;
        }

        if ( ! brackets.isEmpty() ) {
            in_brackets.push( ch );
            index ++;
            // 当字符串是括号里的字母，跳过加入队列
            continue;
        }

        result->enQueue(ch);
    }

    delete &brackets,
            trans_result;
}

/// 通过字符串操作实现的魔王语言的解密
/// \param str 解密的结果字符串
void second_way( char *str ) {
    int len = strlen(str),index = 0;
    while ( index < len ) {
        // A,B 这种“魔王的词汇”
        if ( 'A' <= str[index] && str[index] <= 'Z' ) {
            std::cout << rule_one_v1(str[index]);
        }

        // 这种情形 (...)
        if ( str[index] == 40 ) {
            char to_trans[20];
            int current_pos = 0;
            // 跳过符号 (
            index ++;
            while (str[index] != 41) {
                to_trans[current_pos] = str[index];
                index ++;
                current_pos ++;
            }
            // 结束该字符串
            to_trans[current_pos] = '\0';
            std::cout << real_rule_two(to_trans);
        }
        index ++;
    }
}