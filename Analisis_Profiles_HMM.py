# Secuencias alineadas
sequences = [
    "VGA--HAGEY",
    "V----NVDEV",
    "VEA--DVAGH",
    "VKG------D",
    "VYS--TYETS",
    "FNA--NIPKH",
    "IAGADNGAGY"
]

num_sequences = len(sequences)
num_columns = len(sequences[0])
match_states = []

for col in range(num_columns):
    num_residues = sum(seq[col] != '-' for seq in sequences)
    if num_residues > num_sequences / 2:
        match_states.append(col)

symbol_counts = [{} for _ in range(num_columns)]
symbols = "ACDEFGHIKLMNPQRSTVWY"
pseudo_count = 1 
num_symbols = len(symbols)

for col in match_states:
    for seq in sequences:
        symbol = seq[col]
        if symbol != '-':
            if symbol in symbol_counts[col]:
                symbol_counts[col][symbol] += 1
            else:
                symbol_counts[col][symbol] = 1

symbol_probabilities = [{} for _ in range(num_columns)]
total_count = len(sequences)

for col in match_states:
    for symbol in symbols:
        observed_count = symbol_counts[col].get(symbol, 0)
        numerator = observed_count + pseudo_count
        denominator = total_count + num_symbols * pseudo_count
        symbol_probabilities[col][symbol] = f"{numerator}/{denominator}"

print("Probabilidades de emisión:")
for col in match_states:
    print(f"Columna {col}: {symbol_probabilities[col]}")

print("Transiciones:")
for col in range(len(match_states) - 1):
    sum_gaps = 0
    for seq_idx in range(num_sequences):
        if sequences[seq_idx][match_states[col + 1]] == '-':
            sum_gaps += 1
    num_matches = num_sequences - sum_gaps
    transition_match_to_match = f"{num_matches + 1}/{num_sequences + 3}"
    transition_match_to_delete = f"{sum_gaps + 1}/{num_sequences + 3}"
    transition_match_to_insert = "1/10"
    print(f"Columna {match_states[col]}: transición match to match: {transition_match_to_match}, "
          f"transición match to delete: {transition_match_to_delete}, "
          f"y transición match to insert: {transition_match_to_insert}")
