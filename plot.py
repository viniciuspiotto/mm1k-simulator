import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.interpolate import griddata

def plot_3d_curves(csv_path, metric_folder, graphic_name):
    df = pd.read_csv(csv_path)

    fig = plt.figure(figsize=(20, 8))
    ax = fig.add_subplot(111, projection='3d')

    curve_types = df['simulation_type'].unique()

    cmap = plt.cm.viridis
    colors = [cmap(i / len(curve_types)) for i in range(len(curve_types))]

    for i, curve in enumerate(curve_types):
        df_curve = df[df['simulation_type'] == curve]
        X = df_curve['service_time_avarage'].values
        Y = np.full_like(X, curve)
        Z = df_curve['metric_value'].values

        ax.plot(
            X, Y, Z,
            color=colors[i],
            label=f"{curves_name[curve]}",
            linewidth=2
        )

    ax.set_xlabel('Taxa de Serviço')
    ax.set_ylabel('Política de escalonamento')
    ax.set_zlabel(graphic_labels[graphic_name])
    ax.set_title(graphic_labels[graphic_name])
    ax.legend()
    ax.view_init(elev=30, azim=250)

    output_dir = os.path.join("plots/generic", metric_folder)
    os.makedirs(output_dir, exist_ok=True)

    plt.savefig(
        os.path.join(output_dir, f"{graphic_name}.jpg"),
        dpi=150,
        bbox_inches="tight"
    )
    plt.close()

def plot_3d_policies(graphic_name, graphic_labels):

    policies = ["longer_Wait", "round_robin", "avarage_lateness"]

    policy_labels = {
        "longer_Wait": "Maior Tempo Esperando",
        "round_robin": "Round Robin",
        "avarage_lateness": "Média de Atraso"
    }

    dataframes = {}
    for policy in policies:
        path = os.path.join("results/problem", policy, f"{graphic_name}.csv")
        df = pd.read_csv(path)
        df = df.drop_duplicates(subset="queue_size", keep="first")
        dataframes[policy] = df

    fig = plt.figure(figsize=(20, 8))
    ax = fig.add_subplot(111, projection='3d')

    cmap = plt.cm.viridis
    azim_angle = 250  
    elev_angle = 30   

    distances = []
    for i, policy in enumerate(policies):
        df = dataframes[policy]
        x = df["queue_size"].values[-1]
        y = i + 1
        z = df["metric_value"].values[-1]
        distance = (
            np.sin(np.radians(azim_angle)) * x +
            np.cos(np.radians(azim_angle)) * y -
            np.sin(np.radians(elev_angle)) * z
        )
        distances.append((distance, i, policy))

    distances.sort()

    for order_index, _, policy in distances:
        df = dataframes[policy]
        X = df["queue_size"].values
        Y = np.full_like(X, policies.index(policy) + 1)
        Z = df["metric_value"].values

        ax.plot(
            X, Y, Z,
            linewidth=2,
            color=cmap(policies.index(policy) / len(policies)),
            label=policy_labels[policy]
        )

    ax.set_xlabel("Tamanho da Fila")
    ax.set_ylabel("Política de escalonamento", labelpad=10)
    ax.set_yticks([1, 2, 3])
    ax.set_yticklabels(["", "", ""]) 
    ax.set_zlabel(graphic_labels[graphic_name], labelpad=10)
    ax.set_title(graphic_labels[graphic_name])

    ax.view_init(elev=elev_angle, azim=azim_angle)
    ax.legend()

    output_dir = os.path.join("plots/problem/compare")
    os.makedirs(output_dir, exist_ok=True)

    plt.savefig(
        os.path.join(output_dir, f"{graphic_name}_3d.jpg"),
        dpi=150,
        bbox_inches="tight"
    )
    plt.close()

def plot_3d_metric(csv_path, metric_folder, graphic_name):
    df = pd.read_csv(csv_path)

    X = df['queue_size'].values
    Y = (df['window_size'] / df['queue_size']).values
    Z = df['metric_value'].values

    xi = np.linspace(X.min(), X.max(), 100)
    yi = np.linspace(Y.min(), Y.max(), 100)

    XI, YI = np.meshgrid(xi, yi)
    ZI = griddata((X, Y), Z, (XI, YI), method='cubic')

    ZI = np.nan_to_num(ZI, nan=np.nanmin(ZI))

    fig = plt.figure(figsize=(16, 10))
    ax = fig.add_subplot(111, projection='3d')

    ax.plot_surface(
        XI, YI, ZI,
        cmap='viridis',
        edgecolor='k',
        linewidth=0.3,
        alpha=0.9
    )

    ax.set_xlabel("Tamanho da Fila")
    ax.set_ylabel("Tamanho da Jenela")
    ax.set_zlabel(metric_labels.get(graphic_name, graphic_name))
    ax.set_title(metric_labels.get(graphic_name, graphic_name))

    ax.view_init(elev=30, azim=250)

    out_dir = os.path.join("plots/problem", metric_folder)
    os.makedirs(out_dir, exist_ok=True)

    plt.savefig(
        os.path.join(out_dir, f"{graphic_name}.jpg"),
        dpi=150,
        bbox_inches="tight"
    )
    plt.close()

base_folder = "results/generic"
generic_folders = ["high_arrival_time_rate", "low_to_high_arrival_time_rate", "same_arrival_time_rate"]
generic_files = ["average_response_time.csv", "blocking_probability.csv", "througput.csv"]

curves_name = ["Média de Atraso", "Maior Tempo Esperando", "Round Robin"]

graphic_labels = {
    "average_response_time": "Tempo Médio de Espera",
    "blocking_probability": "Probabilidade de Bloqueio",
    "througput": "Throughput (Vazão)"
}

for metric_folder in generic_folders:
    for file in generic_files:
        path = os.path.join(base_folder, metric_folder, file)
        graphic_name = os.path.splitext(file)[0]
        plot_3d_curves(path, metric_folder, graphic_name)

policy_base_folder = "results/problem"
policy_folders = ["longer_Wait", "round_robin", "avarage_lateness"]
policy_files = ["fairness.csv", "recall.csv"]

graphic_labels = {
    "fairness": "Justiça",
    "precision": "Precisão",
    "recall": "Sensibilidade"
}

for file in policy_files:
    graphic_name = os.path.splitext(file)[0]
    plot_3d_policies(graphic_name, graphic_labels)

lateness_folder = "results/problem/avarage_lateness"
lateness_files = ["fairness.csv", "recall.csv"]

metric_labels = {
    "fairness": "Justiça",
    "precision": "Precisão",
    "recall": "Sensibilidade"
}

for file in lateness_files:
    path = os.path.join(lateness_folder, file)
    graphic_name = os.path.splitext(file)[0]
    plot_3d_metric(path, "avarage_lateness", graphic_name)
