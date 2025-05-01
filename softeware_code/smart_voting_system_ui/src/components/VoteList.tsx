import { useContext, useEffect, useState } from "react";
import { AppContext } from "../context/appContext";
import LoadingPhoto from "../assets/loadingPhoto.svg";
export const VoteList = () => {
  const { voteList, candidates, voters } = useContext(AppContext);
  const [voteCounts, setVoteCounts] = useState<Record<string, number>>({});

  // Function to count votes
  const updateVoteCounts = (votes: any) => {
    const counts: {
      [candidate: string]: number;
    } = {};
    Object.values(votes).forEach((candidate: any) => {
      if (counts[candidate]) {
        counts[candidate] += 1;
      } else {
        counts[candidate] = 1;
      }
    });
    setVoteCounts(counts);
  };

  useEffect(() => {
    if (!voteList) return;
    updateVoteCounts(voteList);
  }, [voteList]);

  if (!voteList || !voters || !candidates)
    return (
      <div style={{ width: "100%", display: "grid", placeItems: "center" }}>
        <img src={LoadingPhoto} alt="loading..." />
      </div>
    );

  const totalVoters = Object.keys(voters).length;

  return (
    <div style={{ padding: "0 10px" }}>
      <ul
        style={{
          display: "flex",
          flexDirection: "column",
          gap: 20,
        }}
      >
        {Object.entries(candidates).map(([_, name], i) => (
          <li
            style={{
              listStyle: "none",
              width: "100%",
              maxWidth: 500,
              margin: "0 auto",
            }}
            key={i}
          >
            <div
              style={{
                width: "100%",
                backgroundColor: "#141E46",
                height: 50,
              }}
            >
              <div
                style={{
                  width: `${
                    voteCounts[name as string]
                      ? (voteCounts[name as string] / totalVoters) * 100
                      : 0
                  }%`,
                  backgroundColor: "#00A9FF",
                  height: "100%",
                  display: "flex",
                  alignItems: "center",
                  color: "white",
                  transition: ".5s",
                }}
              >
                <span style={{ marginLeft: "8px", fontWeight: "bold" }}>
                  {name as string}
                </span>
              </div>
            </div>
            <div
              style={{ textAlign: "right", fontWeight: "bold", fontSize: 14 }}
            >
              {voteCounts[name as string] || 0} / {totalVoters}
            </div>
            {/* <span>{name as string}</span>
            <span>{voteCounts[name as string] || 0}</span> */}
          </li>
        ))}
      </ul>
    </div>
  );
};
