CREATE DATABASE IF NOT EXISTS simple_oj DEFAULT CHARSET utf8mb4;

USE simple_oj;

CREATE TABLE IF NOT EXISTS problem (
    id INT PRIMARY KEY AUTO_INCREMENT,
    problem_no VARCHAR(32) NOT NULL UNIQUE,
    title VARCHAR(128) NOT NULL,
    level_name VARCHAR(32) NOT NULL,
    cpu_limit INT NOT NULL,
    mem_limit INT NOT NULL,
    description TEXT NOT NULL,
    header_code MEDIUMTEXT NOT NULL,
    tail_code MEDIUMTEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS submission (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    problem_id INT NOT NULL,
    status INT NOT NULL,
    reason VARCHAR(128) NOT NULL,
    code_path VARCHAR(255) NOT NULL,
    stdout_path VARCHAR(255) DEFAULT '',
    stderr_path VARCHAR(255) DEFAULT '',
    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_submission_problem FOREIGN KEY (problem_id) REFERENCES problem(id),
    INDEX idx_submission_problem_id (problem_id),
    INDEX idx_submission_created_at (created_at)
);
