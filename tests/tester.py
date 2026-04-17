import os
import json
import re
import shutil
import subprocess
import tempfile
import time


def load_output():
    outputs = {}
    output_dir = os.path.join(os.path.dirname(__file__), "output_file")

    if not os.path.exists(output_dir):
        return outputs

    for filename in sorted(os.listdir(output_dir)):
        if filename.endswith(".json"):
            filepath = os.path.join(output_dir, filename)
            try:
                with open(filepath, 'r') as f:
                    outputs[filename] = json.load(f)
            except json.JSONDecodeError as e:
                print(f"Error decoding JSON in {filename}: {e}")
            except Exception as e:
                print(f"Error reading {filename}: {e}")

    return outputs


def load_input():
    inputs = {}
    input_dir = os.path.join(os.path.dirname(__file__), "input_file")

    if not os.path.exists(input_dir):
        return inputs

    for filename in sorted(os.listdir(input_dir)):
        if filename.endswith(".txt"):
            filepath = os.path.join(input_dir, filename)
            try:
                with open(filepath, 'r') as f:
                    inputs[filename] = f.read()
            except Exception as e:
                print(f"Error reading {filename}: {e}")

    return inputs


def compare_output(results, output_tests):
    failed_tests = []

    def split_lines(value):
        cleaned = value.rstrip("\n")
        if cleaned == "":
            return []
        return cleaned.splitlines()

    def match_stream(actual, expected, stream_name):
        regex_key = f"{stream_name}_regex"
        regex_lines_key = f"{stream_name}_regex_lines"

        if regex_lines_key in expected:
            patterns = expected.get(regex_lines_key, [])
            actual_lines = split_lines(actual)
            if len(actual_lines) != len(patterns):
                return False, (
                    f"Expected {regex_lines_key} with {len(patterns)} line(s): {patterns}\n"
                    f"but got {len(actual_lines)} line(s)."
                )
            for idx, pattern in enumerate(patterns):
                if re.fullmatch(pattern, actual_lines[idx]) is None:
                    return False, (
                        f"Line {idx + 1} does not match regex.\n"
                        f"Regex: {pattern}\n"
                        f"Actual: {actual_lines[idx]}"
                    )
            return True, f"Matched {regex_lines_key}: {patterns}"

        if regex_key in expected:
            pattern = expected.get(regex_key, "")
            if re.fullmatch(pattern, actual, flags=re.DOTALL) is None:
                return False, f"Regex mismatch for {regex_key}: {pattern}"
            return True, f"Matched {regex_key}: {pattern}"

        expected_text = expected.get(stream_name, "")
        if actual.strip() != expected_text.strip():
            return False, expected_text
        return True, expected_text

    for filename, (stdout, stderr) in results.items():
        base_name = os.path.splitext(filename)[0]
        output_filename = base_name + ".json"

        if output_filename not in output_tests:
            print(f"Skipping comparison for {filename}: {output_filename} not found in outputs.")
            continue

        expected = output_tests[output_filename]
        stdout_ok, expected_stdout = match_stream(stdout, expected, "stdout")
        stderr_ok, expected_stderr = match_stream(stderr, expected, "stderr")

        if not stdout_ok or not stderr_ok:
            failed_tests.append({
                "test": filename,
                "actual_stdout": stdout,
                "actual_stderr": stderr,
                "expected_stdout": expected_stdout,
                "expected_stderr": expected_stderr
            })

    return failed_tests


def format_output(failed_tests):
    if not failed_tests:
        print("\nAll tests passed!")
        return []

    output = []
    output.append(f"\n{len(failed_tests)} test(s) failed:")
    output.append("=" * 60)

    for i, test in enumerate(failed_tests, 1):
        output.append(f"\nTest {i}: {test['test']}")
        output.append("-" * 60)

        output.append("\nSTDOUT (Actual):")
        output.append(test['actual_stdout'])

        output.append("\nSTDERR (Actual):")
        output.append(test['actual_stderr'])

        output.append("\nSTDOUT (Expected):")
        output.append(test['expected_stdout'])

        output.append("\nSTDERR (Expected):")
        output.append(test['expected_stderr'])

        output.append("=" * 60)

    return output


def run_test(input_tests, output_tests):
    results = {}
    project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    tests_root = os.path.dirname(__file__)
    seed_dir = os.path.join(tests_root, "seed_file")
    server_bin = os.path.join(project_root, "myteams_server")
    client_bin = os.path.join(project_root, "myteams_cli")
    lib_path = os.path.join(project_root, "libs", "myteams")

    env = os.environ.copy()
    env["LD_LIBRARY_PATH"] = lib_path
    env["DYLD_LIBRARY_PATH"] = lib_path

    base_port = 4242

    for index, (filename, content) in enumerate(input_tests.items()):
        port = base_port + index
        base_name = os.path.splitext(filename)[0]
        seed_file = os.path.join(seed_dir, base_name + ".json")

        print(f"Running test: {filename}")
        with tempfile.TemporaryDirectory(prefix="myteams-test-") as run_dir:
            if os.path.exists(seed_file):
                shutil.copyfile(seed_file,
                                os.path.join(run_dir, "myteams_data.json"))
            server_proc = subprocess.Popen(
                [server_bin, str(port)],
                env=env,
                cwd=run_dir,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL
            )
            time.sleep(0.2)

            try:
                client_proc = subprocess.Popen(
                    [client_bin, "127.0.0.1", str(port)],
                    env=env,
                    cwd=run_dir,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    text=True
                )

                if client_proc.stdin is not None:
                    command_lines = [line for line in content.splitlines() if line.strip()]
                    for line in command_lines:
                        client_proc.stdin.write(line + "\n")
                        client_proc.stdin.flush()
                        time.sleep(0.2)
                time.sleep(1.2 + 0.2 * len(command_lines))

                if client_proc.stdin is not None:
                    client_proc.stdin.close()

                stdout, stderr = client_proc.communicate(timeout=8)
                results[filename] = (stdout, stderr)

            except subprocess.TimeoutExpired:
                client_proc.kill()
                results[filename] = ("", "Error: Client Timeout")
            except Exception as e:
                results[filename] = ("", f"Error: {str(e)}")
            finally:
                server_proc.terminate()
                try:
                    server_proc.wait(timeout=2)
                except subprocess.TimeoutExpired:
                    server_proc.kill()

    return results


def main():
    print("Running tests...")
    input_tests = load_input()
    output_tests = load_output()
    results = run_test(input_tests, output_tests)
    failed_tests = compare_output(results, output_tests)
    output = format_output(failed_tests)
    print("Tests completed.")
    for test in output:
        print(test)


if __name__ == "__main__":
    main()
