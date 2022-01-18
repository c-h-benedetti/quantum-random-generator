# Quantum Random Generator

![Measurement of the quantum fluctuations of the vacuum](https://qrng.anu.edu.au/wp-content/uploads/2020/08/optics.jpg)

*This project is a C++ interface for the web API provided by The Department of Quantum Science in the [Research School of Physics](https://physics.anu.edu.au/) at the [Australian National University](https://www.anu.edu.au/). You can access the web client [here](https://qrng.anu.edu.au/).*


## Basically ...

A computer can't generate real random numbers, only pseudo-random generation is something possible. However, in this case, actual random numbers are sampled by measuring the quantum fluctuations of the vacuum in a chamber.
These samples follow a uniform distribution, but this little C++ API exposes classes to get uniform, normal & exponential distributions. In order to do so, the CDF of the desired distribution is used to do some reverse sampling from the feed (following a uniform distribution).

Despite the usage of asynchronous double-buffering to get a feed as seamless as possible, these classes are pretty slow. It is mainly due to the combo web request + float parsing. This project is not purposed to be used in an environment where performances are critical, it is just fun to have **actual** random number, and not pseudo-random ones.

## If you are still interested ...

- Web requests were not the main topic of this project, so the [CPR library](https://github.com/libcpr/cpr) was used. It is a wrapper of CurL, OpenSSL, ... presented at the [CppCon 2015](https://www.youtube.com/watch?v=f_D-wD1EmWk).
- As mentioned before, don't expect performances. For example, with the internet connection used during the test phase, more than 2 seconds were required to get 1 sample per pixel for Monte Carlo integration for a 2560x1440 image. QRNG's API only allowing for pulling batches of at most 1024 items doesn't make things any better.
- Only 3 types of distribution are supported for now: Uniform, Normal & Exponential. Implementing your own is very straightforward.