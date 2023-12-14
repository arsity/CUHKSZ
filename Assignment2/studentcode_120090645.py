INIT_FLAG = False  # if true, init() has been called
R_MIN = 0  # in bits per second
R_MAX = 0  # in bits per second
B_MAX = 0.0  # in seconds
AVAILABLE_BITRATES = []  # in bits per second
BITRATE = 0.0  # in bits per second
V = 0  # in seconds
RESERVOIR = 0.0  # in seconds
BUFFER_SIZE = 0  # in bits
RMAX_POINT = 0.0  # in seconds


def student_entrypoint(
    Measured_Bandwidth,
    Previous_Throughput,
    Buffer_Occupancy,
    Available_Bitrates,
    Video_Time,
    Chunk,
    Rebuffering_Time,
    Preferred_Bitrate,
):
    """
    This function is called every time a new video chunk is requested.

    Returns:
        The next video rate
    """

    global INIT_FLAG, BITRATE

    if not INIT_FLAG:
        init(
            Measured_Bandwidth,
            Previous_Throughput,
            Buffer_Occupancy,
            Available_Bitrates,
            Video_Time,
            Chunk,
            Rebuffering_Time,
            Preferred_Bitrate,
        )
        INIT_FLAG = True

    BITRATE = video_rate_adaptation_algo(BITRATE, Buffer_Occupancy["time"])
    return BITRATE


def init(
    Measured_Bandwidth,
    Previous_Throughput,
    Buffer_Occupancy,
    Available_Bitrates,
    Video_Time,
    Chunk,
    Rebuffering_Time,
    Preferred_Bitrate,
):
    """
    This function is called at running student_entrypoint first time.
    """

    global R_MIN, R_MAX, AVAILABLE_BITRATES, V, RESERVOIR, BUFFER_SIZE, B_MAX, RMAX_POINT

    # Initialize hyperparams
    AVAILABLE_BITRATES = list(Available_Bitrates.keys())
    AVAILABLE_BITRATES = [int(rate) for rate in AVAILABLE_BITRATES]
    AVAILABLE_BITRATES.sort()
    R_MIN = AVAILABLE_BITRATES[0]
    R_MAX = AVAILABLE_BITRATES[-1]
    V = Chunk["time"]
    BUFFER_SIZE = Buffer_Occupancy["size"]
    B_MAX = BUFFER_SIZE / R_MIN
    RESERVOIR = V * (R_MAX / R_MIN)  # paper said >= V(R_max/R_min)
    if RESERVOIR > B_MAX:
        print("WARNING: Available Rate_min is too high, rebuffering is unavoidable.")
        RESERVOIR = B_MAX * 0.5  # Just give it a value

    for i in range(round(2 * RESERVOIR), -1, -1):
        RMAX_POINT = B_MAX - i
        if RMAX_POINT > RESERVOIR:
            break


def video_rate_adaptation_algo(rate_prev, buffer_now):
    """
    This function is called every time a new video chunk is requested.

    Args:
        rate_prev: The previously used video rate, in bits per second
        buffer_now: The current buffer occupancy, in seconds

    Returns:
        The next video rate
    """

    global R_MIN, R_MAX, AVAILABLE_BITRATES

    if rate_prev == 0:
        return R_MIN  # Fast-track the case when rate_prev is 0 for the first chunk

    # Set rate_plus to lowest reasonable rate
    if rate_prev == R_MAX:
        rate_plus = R_MAX
    else:
        rate_plus = min([rate for rate in AVAILABLE_BITRATES if rate >= rate_prev])
    # Set rate_minus to the highest reasonable rate
    if rate_prev == R_MIN:  # Consider the case when rate_prev is 0 for the first chunk
        rate_minus = R_MIN
    else:
        rate_minus = max([rate for rate in AVAILABLE_BITRATES if rate <= rate_prev])

    # Select bitrate according to mapping func result
    if mapping_func(buffer_now) >= rate_plus:
        rate_next = max(
            [rate for rate in AVAILABLE_BITRATES if rate <= mapping_func(buffer_now)]
        )
    elif mapping_func(buffer_now) <= rate_minus:
        rate_next = min(
            [rate for rate in AVAILABLE_BITRATES if rate >= mapping_func(buffer_now)]
        )
    else:
        rate_next = rate_prev

    return rate_next


def mapping_func(buffer_now):
    """
    The mapping function between buffer occupancy and bitrate.

    Args:
        buffer_now: The current buffer occupancy, in seconds

    Returns:
        The corresponding bitrate
    """

    global RESERVOIR, B_MAX, R_MIN, R_MAX, RMAX_POINT

    if buffer_now <= RESERVOIR:
        return R_MIN
    elif buffer_now >= RMAX_POINT:
        return R_MAX
    else:
        return R_MIN + (R_MAX - R_MIN) * (buffer_now - RESERVOIR) / (
            RMAX_POINT - RESERVOIR
        )
