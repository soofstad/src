/* Add a dispersive event to a seismic profile */

#include <rsf.h>
#include "addevent.h"

/////////////////////////////////////////////////////

int main(int argc,char**argv)
{
	void* h=NULL;
	sf_file in, out;
	float w0, f0, t0, v0, a0, qv, qa, wvp[4];
	int event, wvtype, a0ref;
	float d1, d2, o1, o2;
	int n1, n2, i1, i2, nfft;
	sf_complex * buf;

	sf_init(argc,argv);

	in  = sf_input("in");
	out = sf_output("out");

    if (!sf_histint(in, "n1", &n1)) sf_error("No n2= in input");
    if (!sf_histint(in, "n2", &n2)) sf_error("No n2= in input");
    if (!sf_histfloat(in, "d1", &d1)) sf_error("No d1= in input");
    if (!sf_histfloat(in, "d2", &d2)) sf_error("No d2= in input");
    if (!sf_histfloat(in, "o1", &o1)) sf_error("No o1= in input");
    if (!sf_histfloat(in, "o2", &o2)) sf_error("No o2= in input");

	if(!sf_getint("wvtype", &wvtype)) wvtype =0;
	/* 0: ricker; x: not support */
	switch(wvtype)
	{
	case 0:
		if(!sf_getfloat("w0", &w0) ) w0 = 35.0;
		/* central frequency of Ricker wavelet */
		wvp[0] = w0;
		break;
	}

	if(!sf_getint("event", &event)) event =2;
	/* 0: linear; 1: parabolic; 2:hyperbolic */
	if(!sf_getint("nfft", &nfft)) nfft = n1*2;
	/* fft length */
	if(!sf_getfloat("t0", &t0)) t0 =0.3;
	/* event travel time at x=0 */
	if(!sf_getfloat("v0", &v0)) v0 =1500.0;
	/* event velocity at x=0, for reference frequency f0 */
	if(!sf_getfloat("a0", &a0)) a0 =1.0;
	/* event amplitude at t=a0ref (x=0) */
	if(!sf_getfloat("qv", &qv)) qv = -1.0;
	/* Q factor for velocity dispersion */
	if(!sf_getfloat("qa", &qa) ) qa = qv;
	/* Q factor for amplitude attenuation */
	if(!sf_getfloat("f0", &f0)) f0 = w0;
	/* reference frequency for velocity dispersion and amplitude attenuation */
	if(!sf_getint("a0ref", &a0ref) ) a0ref = 0;
	/* reference point for a0: 0 - t0; 1 - a0 */

	h = sf_addevent_init(nfft, o1,d1,
		wvtype, wvp,
		event, t0, v0, a0,
		qv, qa, f0, a0ref);

	buf = sf_complexalloc(n1);

	for(i2=0;i2<n2;i2++)
	{
		sf_complexread(buf, n1, in);
		sf_addevent(h , d2*i2+o2, buf);
		sf_complexwrite(buf, n1, out);
	}
	sf_addevent_release(h);

	return 0;
}


