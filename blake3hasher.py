import blake3
import os
import sys

def hash_file(file_path, full=False, max_bytes=50 * 1024):
    """
    Hash the first `max_bytes` of the file or the whole file if `full=True`.
    Returns the BLAKE3 hex digest.
    """
    if not os.path.isfile(file_path):
        raise FileNotFoundError(f"File not found: {file_path}")

    file_size = os.path.getsize(file_path)
    to_read = file_size if full else min(file_size, max_bytes)

    with open(file_path, "rb") as f:
        data = f.read(to_read)
        digest = blake3.blake3(data).hexdigest()
    return digest

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Usage: python {sys.argv[0]} <file_path> [--full]")
        sys.exit(1)

    file_path = sys.argv[1]
    full_hash = "--full" in sys.argv

    try:
        hash_result = hash_file(file_path, full=full_hash)
        print(f"BLAKE3 hash: {hash_result}")
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

