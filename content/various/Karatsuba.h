/**
 * Author: Simon Lindholm
 * Date: 2015-04-22
 * Source: me
 * Description: Faster-than-naive convolution of two sequences: $c[x] = \sum a[i]b[x-i]$. Does not handle sequences of very different length well.
 * Time: O(N^{1.6})
 * Status: tested
 */
#pragma once
#pragma GCC target ("avx,avx2")
#pragma GCC optimize ("Ofast") // makes things 5x faster
#include <vector>
using namespace std;

typedef int T; typedef vector<T> vt; // (short is 30% faster)

void kar(int len, T* ap, T* bp, T* out) {
#define i(A,a,b) rep(i,0,A) i[a] += i[b]
	if (len < 128) {
		rep(i,0,len) rep(j,0,len) out[i+j] += ap[i] * bp[j];
		return;
	}
	int B = len / 2, C = len - B;
	vt t(2*len + 2);
	T *L = &t[0], *R = L + C;
	T *st = L + 2, *temp = st + 2*B;
	kar(B, ap, bp, st);
	kar(C, ap+B, bp+B, temp);
	i(2*len, out, st);
	i(2*B, temp, st);
	rep(i,0,2*C) temp[i] = -temp[i], L[i] = 0;
	i(B, L, ap); i(C, L, ap+B);
	i(B, R, bp); i(C, R, bp+B);
	kar(C, L, R, temp);
	i(2*C, out+B, temp);
}

vt conv(vt a, vt b) {
	if (sz(a) < sz(b)) a.swap(b);
	int s = max(sz(a) + sz(b) - 1, 0);
	vt out(sz(a)*2);
	b.resize(sz(a));
	kar(sz(a), a.data(), b.data(), out.data());
	out.resize(s);
	return out;
}
