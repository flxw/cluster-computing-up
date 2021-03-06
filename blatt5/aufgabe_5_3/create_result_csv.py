#!/usr/bin/env python

from datetime import datetime, timedelta
import pandas as pd
import os

def create_csv(benchmark_file_name, output_file_name, keep_duplicate_results, sort_by):
    content = ""
    with open(benchmark_file_name) as f:
        content = f.read()

    df_lines = []
    runs = content.split("###")[1:]
    for run in runs:
        lines = run.split("\n")[1:-1]
        run_args = lines[0].split("\t")
        run_results = lines[1].split("\t")
        t = datetime.strptime(lines[2].split("\t")[1], "%Mm%S.%fs")
        delta = timedelta(hours=t.hour, minutes=t.minute, seconds=t.second, microseconds=t.microsecond)
        
        df_lines = df_lines + [pd.to_numeric(run_args + run_results + [delta.total_seconds()])]
        
    df = pd.DataFrame(
        data = df_lines,
        columns=[
            "num_samples",
            "num_threads",
            "num_pos_samples",
            "num_neg_samples",
            "approx_pi",
            "relative_error",
            "exec_time"
        ]
    )

    if keep_duplicate_results == False:
        df = df.drop_duplicates(subset=["num_samples", "num_threads"], keep="last")
    if sort_by != "":
        df.sort_values(by=sort_by, inplace=True)
    df.to_csv(output_file_name, index=False)

if __name__ == '__main__':
    if len(os.sys.argv) != 5:
        print "Usage: create_result_csv.py <benchmark_file_name> <output_csv_filename> <keep_duplicate_results> <sort_by>"

    input_file_name = os.sys.argv[1]
    output_csv_file_name = os.sys.argv[2]
    keep_duplicate_results = True if os.sys.argv[3] == "true" else False
    sort_by = os.sys.argv[4] if (os.sys.argv[4] in [
            "num_samples",
            "num_threads",
            "num_pos_samples",
            "num_neg_samples",
            "approx_pi",
            "relative_error",
            "exec_time"
        ]) else ""

    create_csv(input_file_name, output_csv_file_name, keep_duplicate_results, sort_by)