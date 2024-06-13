#ifdef _WIN32

#else
long long getTime_ns(){

	timespec timeSpec;
	clock_gettime(CLOCK_MONOTONIC_RAW, &timeSpec);

	return (long int)timeSpec.tv_sec * 1000000000 + timeSpec.tv_nsec;

}
#endif

long long getTime_us(){
	return getTime_ns() / 1000;
}

long long getTime_ms(){
	return getTime_ns() / 1000000;
}

